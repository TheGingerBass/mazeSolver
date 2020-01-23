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
    /*Solution(int color_in, int height_in, int width_in) {
        colors = color_in;
        height = height_in;
        width = width_in;
    }*/
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
    void resize2dVec(vector<vector<int>> &vec, int rows, int cols)
    {
        vec.resize(rows);
        for (auto &it : vec)
        {
            it.resize(cols);
        }
    }
    void readInput() {
        cin >> colors;
        cin >> height;
        cin >> width; // Reads in first three ints
        inputCheck();
        puzzle.resize(height, vector<char>(width, '.'));

        backtrack.resize(colors + 1, vector<vector<char>>(height, vector<char>(width, '.')));
        
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
        if (c == '^') {
            return 0;
        }
        return (c - 'a') + 1;
    }
    char int2Char(int color) {
        if (color == 94) {
            return '^';
        }
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
            (char2Int(current) > 0 && char2Int(current) < 27) ||
            (char2Int(current) > -32 && char2Int(current) < -5)) {
            return true;
        }
        return false;
    }
    bool checkNorth(State current) {
        if (current.row == 0) {
            return false;
        }
        char north = puzzle[current.row - 1][current.col];
        if (!isValidCharacter(north)) {
            return false;
        }
        if ((char2Int(north) > 0 && char2Int(north) < 27 || (char2Int(north)
        > -32 && char2Int(north) < -5)) && (char2Int(north) - 32 == 
            char2Int(current.color)) || char2Int(north) - 32 ==
            char2Int(current.color)) {
            if (backtrack[char2Int(current.color)][current.row - 1][current.col] != '.') {
                return false;
            }
            return true;
        }
        if (north == '?' || north == '.' || north == '@') {
            if (backtrack[char2Int(current.color)][current.row - 1][current.col] != '.') {
                return false;
            }
            return true;
        }
        return false;
    }
    bool checkEast(State current) {
        if (current.col == width - 1) {
            return false;
        }
        char east = puzzle[current.row][current.col + 1];
        if (!isValidCharacter(east)) {
            return false;
        }
        if ((char2Int(east) > 0 && char2Int(east) < 27 || (char2Int(east)
        > -32 && char2Int(east) < -5)) && (char2Int(east) - 32 == 
            char2Int(current.color) || char2Int(east) - 32 ==
            char2Int(current.color))) {
            if (backtrack[char2Int(current.color)][current.row][current.col + 1] != '.') {
                return false;
            }
            return true;
        }
        if (east == '?' || east == '.' || east== '@') {
            if (backtrack[char2Int(current.color)][current.row][current.col + 1] != '.') {
                return false;
            }
            return true;
        }
        return false;
    }
    bool checkSouth(State current) {
        if (current.row == height - 1) {
            return false;
        }
        char south = puzzle[current.row + 1][current.col];
        if (!isValidCharacter(south)) {
            return false;
        }
        if (((char2Int(south) > 0 && char2Int(south) < 27) || (char2Int(south) 
        > -32 && char2Int(south) < -5)) && (char2Int(south) - 32 == 
            char2Int(current.color) || char2Int(south) + 32 ==
            char2Int(current.color))) {
            if (backtrack[char2Int(current.color)][current.row + 1][current.col] != '.') {
                return false;
            }
            return true;
        }
        if (south == '?' || south == '.' || south == '@') {
            if (backtrack[char2Int(current.color)][current.row + 1][current.col] != '.') {
                return false;
            }
            return true;
        }
        return false;
    }
    bool checkWest(State current) {
        if (current.col == 0) {
            return false;
        }
        char west = puzzle[current.row][current.col - 1];
        if (!isValidCharacter(west)) {
            return false;
        }
        if ((char2Int(west) > 0 && char2Int(west) < 27 || (char2Int(west)
        > -32 && char2Int(west) < -5)) && (char2Int(west) - 32 != 
            char2Int(current.color) || char2Int(west) + 32 ==
            char2Int(current.color))) {
            if (backtrack[char2Int(current.color)][current.row][current.col - 1] != '.') {
                return false;
            }
            return true;
        }
        if (west == '?' || west == '.' || west == '@') {
            if (backtrack[char2Int(current.color)][current.row][current.col - 1] != '.') {
                return false;
            }
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
            backtrack[char2Int(current.color)][current.row - 1][current.col] =
                'S';
            if (findQuestionMark(puzzle[current.row - 1][current.col])) {
                end = true;
                return;
            }
        }
        if (checkEast(current)) {
            temp.col = current.col + 1;
            temp.row = current.row;
            temp.color = current.color;
            reachable.push_back(temp);
            backtrack[char2Int(current.color)][current.row][current.col + 1] =
                'W';
            if (findQuestionMark(puzzle[current.row][current.col + 1])) {
                end = true;
                return;
            }
        }
        if (checkSouth(current)) {
            temp.col = current.col;
            temp.row = current.row + 1;
            temp.color = current.color;
            reachable.push_back(temp);
            backtrack[char2Int(current.color)][current.row + 1][current.col] =
                'N';
            if (findQuestionMark(puzzle[current.row + 1][current.col])) {
                end = true;
                return;
            }
        }
        if (checkWest(current)) {
            temp.col = current.col - 1;
            temp.row = current.row;
            temp.color = current.color;
            reachable.push_back(temp);
            backtrack[char2Int(current.color)][current.row][current.col - 1] =
                'E';
            if (findQuestionMark(puzzle[current.row][current.col - 1])) {
                end = true;
                return;
            }
        }
    }
    bool isLetter(char spot) {
        if (char2Int(spot) > 0 && char2Int(spot) < 27) {
            return true;
        }
        return false;
    }
    bool isButton(State current) {
        if ((isLetter(puzzle[current.row][current.col]) || 
            puzzle[current.row][current.col] == '^') &&
            puzzle[current.row][current.col] != current.color) {
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
                if (mode == "queue") {
                    reachable.pop_front();
                }
                reachable.pop_back();
                if (isButton(current) && !findInReachable(current)) {
                    tempState.color = puzzle[current.row][current.col];
                    tempState.col = current.col;
                    tempState.row = current.row;
                    reachable.push_back(tempState);
                    backtrack[char2Int(tempState.color)][current.row][current.col] 
                        = tempState.color;
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