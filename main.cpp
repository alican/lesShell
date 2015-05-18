#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <array>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
// get user name
#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "JobController.h"
#include "Job.h"



using namespace std;


vector<pid_t> processlist;
JobController jbController;
int status;

void sigint_handler(int signum){            // cmd + C
    pid_t pid = jbController.getFgJob();
    int killreturn = killpg(pid, SIGINT);
}

void sigtstp_handler(int signum){         // cmd + Z
    pid_t pid = jbController.getFgJob();
    killpg(pid, SIGTSTP);
}

void childSignalHandler(int signum) {      // wenn child beendet wurde
    int status;
    pid_t pid;
    pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
}


// ############################### BUILD IN Command ################################
void command_help(void) {
    cout << "help?" << endl;
}
void command_fg(void) {
    pid_t pid = jbController.getFgJob();
    cout << pid;
    int killreturn = killpg(pid, SIGCONT);
    pid = waitpid(pid, &status, 0);

}
void command_bg(void) {
    pid_t pid = jbController.getFgJob();
    cout << pid;
    int killreturn = killpg(pid, SIGCONT);
}
void command_exit(void) {
    exit(0);
}
typedef void (*CommandFunction)(void); // function pointer type
const map<std::string, CommandFunction> CommandMap = {
        {"logout", &command_exit},
        {"help", &command_help},
        {"fg", &command_fg},
        {"bg", &command_bg},
};

int exeBuildInCommands(const std::string& pFunction)
{
    map<std::string, CommandFunction>::const_iterator iter = CommandMap.find(pFunction);
    if (iter == CommandMap.end()) {
        return 0;
    }
    (*iter->second)();
}


string getUserName(){
    register struct passwd *pw;
    register uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);
    return string(pw->pw_name);
}

string printPrompt(){
    string homedir = getenv("HOME");
    return "[" + getUserName() + "] $ ";
}

void printWelcome(){
    cout << "-- lesShell v0.01-- " << endl;
    cout << getenv("HOME") << endl;
    cout << getenv("LOGNAME") << endl;
    cout << "Angemeldet als: " + getUserName() << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~";
    cout << "\n\n";
}




int executeCommand(vector<string> &args, bool background){
    int options = WUNTRACED;

    pid_t childPID = fork();
    switch(childPID){
        case 0:{
            const char** argV = new const char*[args.size()+1];
            for (int k = 0; k < args.size(); k++ ){
                argV[k] = args[k].c_str();
            }
            argV[args.size()] = NULL;
            setpgid(0, 0);              // child bekommt neue Gruppe
            execvp(argV[0], (char **)argV);
            return -1;
        }
        case -1:{
            perror( "Internal error: cannot fork." );
            return -1;
        }
        default:{
            signal(SIGINT, sigint_handler);
            signal(SIGTSTP, sigtstp_handler);

            Job *newjob = new Job(args[0], childPID);
            jbController.addJob(newjob);                // Prozess kommt in die JobListe

            if (background){
                options = WNOHANG|WUNTRACED;
                signal(SIGCHLD, childSignalHandler);
            }
            if (waitpid(childPID, &status, options) < 0) // Parent process is waiting for the child.
            {
                perror( "Internal error: cannot wait for child." );
                return -1;
            }
        }
            break;
    }
};

bool has_only_spaces(const string &str) {
    return str.find_first_not_of (' ') == str.npos;
}


void parse(string line, vector<string> &args){

    std::istringstream iss(line);
    std::string arg;

    while(getline(iss, arg, ' '))
    {
        if (!has_only_spaces(arg)){
            args.push_back(arg);
            //std::cout << arg << std::endl;
        }
    }
}

int main(int argc, char **argv, char **envp) {

    char* input, shell_prompt[100];
    printWelcome();

    for(;;){
        vector<string> args;
        bool background = false;
        const char *prompt = printPrompt().c_str();

        input = readline(prompt);

        snprintf(shell_prompt, sizeof(shell_prompt), "%s:%s $ ", getenv("USER"), getcwd(NULL, 1024));

        if (!input){
            break;
        }

        add_history(input);

        parse(input, args); // token to vector

        if(args.back() == "&"){
            args.pop_back();
            background = true;
        }

        if (!args.empty()){
            if (exeBuildInCommands(args[0])){
            }else{
                executeCommand(args, background);
            }
        }
        free(input);

    }

    return 0;
}