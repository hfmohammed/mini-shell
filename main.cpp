#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <cstdlib>
using namespace std;


void sigint_handler(int signum) {
    cout << "\n\nExiting... Have a great day!" << endl;
    exit(0);
}

vector<char*> tokenize(string str) {
    istringstream iss(str);
    vector<char*> tokens;
    string token;

    while (iss >> token) {
        char* token_cstr = new char[token.length() + 1];
        strcpy(token_cstr, token.c_str());
        tokens.push_back(token_cstr);
    }

    return tokens;
}

int executeCommand(vector<char*> tokens) {
    // process external command
    if (execvp(tokens[0], tokens.data()) == -1) {
        perror("Error: command couldn't execute");
        return 0;
    }

    return 0;
}

int main() {
    string input;

    while(1) {
        // get input
        cout << "mysh> ";
        getline(cin, input);
        if (input.empty()) continue;

        // tokenize the command
        vector<char*> tokens = tokenize(input);

        // process cd
        if (strcmp(tokens[0], "cd") == 0) {
            if (tokens.size() > 1) {
                if (chdir(tokens[1]) == -1) {
                    perror("couldn't change directory");
                }
            } else {
                cout << "\n\tcd:\tExpected arguments 2 but got 1\n\n";
            }
        }

        // process exit
        else if (strcmp(tokens[0], "exit") == 0) {
            if (tokens.size() != 1) {
                cout << "\n\texit:\tExpected 1 argument\n\n";
            } else {
                return 0;
            }
        }

        // process help
        else if (strcmp(tokens[0], "help") == 0) {
            if (tokens.size() != 1) {
                cout << "\n\thelp:\tExpected 1 argument\n\n";
            } else {
                cout << "\nUSAGE:\n\texit:\t\t\texit the shell\n\tcd [directory]:\t\tmove to [directory]\n\t[external command]:\trun the command\n\thelp:\t\t\tshow manual\n\n";
            }
        }

        else {
            // fork process
            pid_t pid = fork();
            if (pid == 0) {     // child process
                // execute command
                if (executeCommand(tokens) == -1)  {
                    exit(42);
                } exit(0);
            } else if (pid > 0) {   // parent process
                waitpid(pid, nullptr, 0);
                for (char* token : tokens) delete[] token;
            } else {
                perror("Error: fork failed.");
                exit(1);
            }
        }
    }
    
    return 0;
}