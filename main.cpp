#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
// get user name
#include <unistd.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>


using namespace std;


void signal_callback_handler(int signum){
    cout << "signum: " << signum << endl;

}


const array<string, 2> buildInCommands = {
        "cd",
        "help"
};


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



bool isBuildInCommand(string command){
    for (string cmd : buildInCommands){
        if (cmd == command) return true;
    }
    return false;
}

void executeBuildInCommand(vector<string> &arguments){};


int executeCommand(vector<string> &args, bool background){
    pid_t childPID = fork();
    switch(childPID){
        case 0:{
            const char** argV = new const char*[args.size()+1];
            for (int k = 0; k < args.size(); k++ ){
                argV[k] = args[k].c_str();
            }
            argV[args.size()] = NULL;

            execvp(argV[0], (char **)argV);
            return -1;
        }
        case -1:{
            perror( "Internal error: cannot fork." );
            return -1;
        }
        default:{
            if (background){
                cout << childPID << endl;
            }else if ( waitpid( childPID, 0, 0 ) < 0 ) // Parent process is waiting for the child.
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
    signal(SIGUSR1, signal_callback_handler);

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
            if (isBuildInCommand(args[0])){
                cout << "is build in" << endl;

            }else{
                executeCommand(args, background);
            }
        }
        free(input);

    }

    return 0;
}