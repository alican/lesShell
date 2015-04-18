#include <iostream>



using namespace std;

void printPromt(){
    cout << "$ ";
}

int main() {
    string input = "test";

    while(input != "exit"){
        printPromt();

        getline(cin, input);
        cout << input << endl;

    }

    return 0;
}