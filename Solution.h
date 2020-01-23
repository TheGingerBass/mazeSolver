// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725
// Noah Bonello

#include <string>
#include <iostream>
#include <deque>
#include <vector>
#include <stack>
#include <getopt.h>
using namespace std;

struct State {
    char color;
    int row;
    int col;
};

class Solution {
private:
    string mode = "";
    string type = "";
    unsigned int colors = 0;
    unsigned int height = 0;
    unsigned int width = 0;
    bool end = false;
    vector < vector<char>> puzzle;
    vector < vector < vector <char>>> backtrack;
    deque<State> reachable;
public:
    void printHelp(char * argv[]) { // TODO: this is copy pasted from project 0, update to have right info
        cout << "Usage: " << argv[0] << " -h|-q|-s|[-o map|list]\n";
        cout << "This program is to find if there is a solution to a\n";
        cout << "given puzzle, recording the solution if there is one,\n";
        cout << "and letting the user know if there is no solution.\n";
    } // printHelp()
    // Process the command line; the only thing we need to return is the mode
    // when the user specifies the -m/--mode option.
    string getoptPrep(int argc, char * argv[]) {
        bool modeSpecified = false;
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
    /////////////////////////////////////////////////////////////////////////
    void inputCheck() {
        if (colors < 0 || colors > 26) {
            cerr << "too few or too many colors\n";
            exit(1);
        }
        else if (height < 1 || width < 1) {
            cerr << "height or width are smaller than 1\n";
            exit(1);
        }
    }
    void readJunk() {
        string junk;
        getline(cin, junk);
    }
    void readInput() {
        cin >> colors;
        cin >> width;
        cin >> height; // Reads in first three ints
        inputCheck();
        puzzle.resize(height, vector<char>(width, '.'));
        backtrack.resize(height, vector<vector<char>>(width,
            vector<char>(colors, '.')));
        vector <char> row(width);
        char symbol_in;
        cin >> symbol_in;
        // This next chunk reads in comments and then the first line after
        // the last comment. 
        while (symbol_in == '/') {
            readJunk();
            cin >> symbol_in;
        } // while
        int temp = -1;
        for (int i = 1; i < width; ++i) {
            row[i - 1] = symbol_in;
            cin >> symbol_in;
            temp = i;
        }
        row[temp] = (symbol_in);
        puzzle[0] = (row);
        // Reads in the rest of the initial puzzle
        for (int i = 1; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                cin >> symbol_in;
                row[j] = (symbol_in);
            } // for j
            puzzle[i] = (row);
        } // for i
    }
    /////////////////////////////////////////////////////////////////////////
    void print2DVector() { // For debugging purposes only
        for (int i = 0; i < puzzle.size(); i++) {
            for (int j = 0; j < puzzle[i].size(); j++) {
                cout << puzzle[i][j] << " ";
            }
            cout << endl;
        }
    }
    int char2Int(char c) {
        return (c - 'a') + 1;
    }
    char int2Char(int color) {
        return (color + 'a') - 1;
    }
    State findStart() {
        State found;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (puzzle[i][j] == '@') {
                    found.color = '^';
                    found.row = i;
                    found.col = j;
                    return found;
                }
            }
        }
    }
    bool isValidCharacter(char current) {
        if (current == '.' || current == '?' || current == '#' || 
            current == '^' ||
            (char2Int(current) > 64 && char2Int(current) < 91) ||
            (char2Int(current) > 96 && char2Int(current) < 123)) {
            return true;
        }
        return false;
    }
    bool checkNorth(State current) {
        char north = puzzle[current.row - 1][current.col];
        if (current.row == 0 || (!isValidCharacter(north))) {
            return false;
        }
        if ((char2Int(north) > 64 && char2Int(north) < 91) && char2Int(north) 
            + 32 == char2Int(current.color)) {
            return true;
        }
        return false;
    }
    bool checkEast(State current) {
        char east = puzzle[current.row][current.col + 1];
        if (current.col == width - 1 || (!isValidCharacter(east))) {
            return false;
        }
        if ((char2Int(east) > 64 && char2Int(east) < 91) && char2Int(east)
            + 32 == char2Int(current.color)) {
            return true;
        }
        return false;
    }
    bool checkSouth(State current) {
        char south = puzzle[current.row + 1][current.col];
        if (current.col == height - 1 || (!isValidCharacter(south))) {
            return false;
        }
        if ((char2Int(south) > 64 && char2Int(south) < 91) && char2Int(south)
            + 32 == char2Int(current.color)) {
            return true;
        }
        return false;
    }
    bool checkWest(State current) {
        char west = puzzle[current.row][current.col - 1];
        if (current.col == width - 1 || (!isValidCharacter(west))) {
            return false;
        }
        if ((char2Int(west) > 64 && char2Int(west) < 91) && char2Int(west)
            + 32 == char2Int(current.color)) {
            return true;
        }
        return false;
    }
    bool findQuestionMark(char space) {
        if (space == '?') return true;
        return false;
    }
    void checkDirectionAndPush(State current) {
        State temp;
        if (checkNorth(current)) {
            temp.col = current.col;
            temp.row = current.row - 1;
            temp.color = current.color;
            reachable.push_back(temp);
            if (findQuestionMark(puzzle[current.col][current.row - 1])) {
                end = true;
                return;
            }
        }
        if (checkEast(current)) {
            temp.col = current.col + 1;
            temp.row = current.row;
            temp.color = current.color;
            reachable.push_back(temp);
            if (findQuestionMark(puzzle[current.col + 1][current.row])) {
                end = true;
                return;
            }
        }
        if (checkSouth(current)) {
            temp.col = current.col;
            temp.row = current.row + 1;
            temp.color = current.color;
            reachable.push_back(temp);
            if (findQuestionMark(puzzle[current.col][current.row + 1])) {
                end = true;
                return;
            }
        }
        if (checkWest(current)) {
            temp.col = current.col - 1;
            temp.row = current.row;
            temp.color = current.color;
            reachable.push_back(temp);
            if (findQuestionMark(puzzle[current.col - 1][current.row])) {
                end = true;
                return;
            }
        }
    }
    bool isLetter(char spot) {
        if (char2Int(spot) > 96 && char2Int(spot) < 123) {
            return true;
        }
        return false;
    }
    bool isButton(State current) {
        if ((isLetter(puzzle[current.col][current.row]) || 
            puzzle[current.col][current.row] == '^') &&
            puzzle[current.col][current.row] != current.color) {
            return true;
        }
        return false;
    }
    bool findInReachable(State current) {
        for (int i = 0; i < reachable.size(); ++i) {
            if (reachable[i].col == current.col &&
                reachable[i].row == current.row &&
                reachable[i].color == current.color) {
                return true;
            }
            return false;
        }
    }
    void solutionFinder() {
        State current = findStart();
        State tempState;
        reachable.push_back(current);
        while (!reachable.empty()) {
            if (!end) {
                current = reachable.front();
                reachable.pop_front();
                if (isButton(current) && !findInReachable(current)) {
                    tempState.color = puzzle[width][height];
                    tempState.col = current.col;
                    tempState.row = current.row;
                    reachable.push_back(tempState);
                }
                checkDirectionAndPush(current);
            }
            else {
                break;
            }
        }
        // do stuff
        
    }
    ///////////////////////////////////////////////////////////////////////////
    void writeOutput();
    ///////////////////////////////////////////////////////////////////////////
};