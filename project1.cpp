// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725
// Noah Bonello

#include "Solution.h"
#include <getopt.h>

string getoptPrep(int argc, char * argv[]);
void printHelp(char * argv[]);

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    string mode =  getoptPrep(argc, argv);
    cout << mode << endl;
    Solution s;
    s.readInput();
    s.print2DVector();
    return 0;
}

// Process the command line; the only thing we need to return is the mode
// when the user specifies the -m/--mode option.
string getoptPrep(int argc, char * argv[]) {
    bool modeSpecified = false;
    string mode;
    string type;
    // These are used with getopt_long()
    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "help", no_argument,         nullptr, 'h' },
        { "queue", no_argument,        nullptr, 'q' },
        { "stack", no_argument,        nullptr, 's' },
        { "output", required_argument, nullptr, 'o' },
        { nullptr, 0,                  nullptr, '\0' }
    };
    while ((choice = getopt_long(argc, argv, "hqso:", long_options, 
        &option_index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(argv);
            exit(0);

        case 'q':
            cout << "case q\n";
            if (mode == "stack") {
                cerr << "there may only be one output mode!!\n";
                exit(1);
            }
            mode = "queue";
            break;

        case 's':
            cout << "case s\n";
            if (mode == "queue") {
                cerr << "there may only be one output mode!!\n";
                exit(1);
            }
            mode = "stack";
            break;

        case 'o':
            type = optarg;
            cout << "case o\n";
            if (type != "map" && type != "list") {
                cerr << "Error: invalid type for output type" << type << "\n";
                exit(1);
            } // if
            modeSpecified = true;
            break;

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);
        } // switch
    } // while
    if (mode != "stack" && mode != "queue") {
        cerr << "mode was never assigned to stack or queue\n";
        exit(1);
    }

    if (!modeSpecified) {
        cerr << "Error: no mode specified" << endl;
        exit(1);
    } // if

    return mode;
    
} // getoptPrep()

void printHelp(char * argv[]) { // TODO: this is copy pasted from project 0, update to have right info
    cout << "Usage: " << argv[0] << " -h|-q|-s|[-o map|list]\n"; 
    cout << "This program is to find if there is a solution to a\n"; 
    cout << "given puzzle, recording the solution if there is one,\n"; 
    cout << "and letting the user know if there is no solution.\n"; 
} // printHelp()