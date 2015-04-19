#include <iostream>
#include <sstream>
#include <vector>
#include <array>

// get user name
#include <unistd.h>
#include <pwd.h>

using namespace std;



std::string getUserName(){
    register struct passwd *pw;
    register uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);
    return string(pw->pw_name);
}

void printPrompt(){
    cout << "[" + getUserName() + "] $ ";
}

void printWelcome(){
    cout << "-- lesShell v0.01-- " << endl;
    cout << "Angemeldet als: " + getUserName() << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~";
    cout << "\n\n";
}

array<string, 2> buildInCommands = {"cd", "help"};


bool isBuildInCommand(string command){
    for (string cmd : buildInCommands){
        if (cmd == command) return true;
    }
    return false;
}

void executeBuildInCommand(vector<string> &arguments){};


void parse(string line, vector<string> &args){

    std::istringstream iss(line);
    std::string arg;

    while(getline(iss, arg, ' '))
    {
        args.push_back(arg);
        std::cout << arg << std::endl;
    }
}



int main(int argc, char **argv, char **envp) {
    printWelcome();

    string line;
    vector<string> args;
    vector<string> histrory;


    while(line != "logout"){
        printPrompt();

        getline(cin, line);
        if (line == "" || line == " ") continue;

        parse(line, args); // token to vector

        if (!args.empty()){
            if (isBuildInCommand(args[0])){
                cout << "is build in" << endl;
                //executeBuildInCommand(args);
            }else{
                cout << "iasd" << endl;

            }
        }
    }

    return 0;
}