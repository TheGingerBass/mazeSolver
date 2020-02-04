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
    bool isQueue = false;
    bool isStack = false;
    string type = "";
    unsigned int colors = 0;
    unsigned int height = 0;
    unsigned int width = 0;
    bool end = false;
    vector < vector<char>> puzzle;
    vector < vector < vector <char>>> backtrack;
    deque<State> reachable;
    vector<State> answer;
    State finalLocation;
    State beginLocation;
public:
    void printHelp(char * argv[]) { 
        cout << "Usage: " << argv[0] << " -h|-q|-s|[-o map|list]" << endl;
        cout << "This program is to find if there is a solution to a" << endl;
        cout << "given puzzle, recording the solution if there is one," << endl;
        cout << "and letting the user know if there is no solution." << endl;
    } // printHelp()
    // Process the command line; the only thing we need to return is the mode
    // when the user specifies the -m/--mode option.
    void getoptPrep(int argc, char * argv[]) {
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
                isQueue = true;
                if (isStack) {
                    cerr << "there may only be one output mode!!" << endl;
                    exit(1);
                }
                break;

            case 's':
                isStack = true;
                if (isQueue) {
                    cerr << "there may only be one output mode!!" << endl;
                    exit(1);
                }
                break;

            case 'o':
                type = optarg;
                if (type != "map" && type != "list") {
                    cerr << "Error: invalid type for output type" << type << endl;
                    exit(1);
                } // if
                modeSpecified = true;
                break;

            default:
                cerr << "Error: invalid option" << endl;
                exit(1);
            } // switch
        } // while
        if (!isStack && !isQueue) {
            cerr << "mode was never assigned to stack or queue" << endl;
            exit(1);
        }

        if (!modeSpecified) {
            type = "map";
        } // if
    } // getoptPrep() 
    /////////////////////////////////////////////////////////////////////////
    void inputCheck() {
        if ((int)colors < 0 || (int)colors > 26) {
            cerr << "too few or too many colors" << endl;
            exit(1);
        }
        else if (height < 1 || width < 1) {
            cerr << "height or width are smaller than 1" << endl;
            exit(1);
        }
    }
    void readJunk() {
        string junk;
        getline(cin, junk);
    }
    void readInput() {
        cin >> colors;
        cin >> height;
        cin >> width; // Reads in first three ints
        char symbol_in;
        inputCheck();
        puzzle.resize(height, vector<char>(width, '.'));

        backtrack.resize(colors + 1, vector<vector<char>>(height, vector<char>(width, '.')));
        
        vector <char> row(width);
        cin >> symbol_in;
        // This next chunk reads in comments and then the first line after
        // the last comment. 
        while (symbol_in == '/') {
            readJunk();
            cin >> symbol_in;
        } // while
        int temp = -1;
        for (unsigned int i = 1; i < width; ++i) {
            row[i - 1] = symbol_in;
            cin >> symbol_in;
            temp = i;
        }
        row[temp] = (symbol_in);
        puzzle[0] = (row);
        // Reads in the rest of the initial puzzle
        for (unsigned int i = 1; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                cin >> symbol_in;
                row[j] = (symbol_in);
            } // for j
            puzzle[i] = (row);
        } // for i
    }
    
    /////////////////////////////////////////////////////////////////////////
    int char2Int(char c) {
        if (c == '^') {
            return 0;
        }
        return (c - 'a') + 1;
    }
    char int2Char(int color) {
        if (color == 0) {
            return '^';
        }
        return static_cast <char>((color + 'a') - 1);
    }
    bool isLetter(char letter) {
        if ((char2Int(letter) > 0 && char2Int(letter) < 27) ||
            (char2Int(letter) > -32 && char2Int(letter) < -5)) {
            return true;
        }
        return false;
    }
    void locateInMap(char hunt, State& found) {
        
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                if (puzzle[i][j] == hunt) {
                    if (found.col == -1) {
                        found.color = '^';
                        found.row = i;
                        found.col = j;
                    }
                    else if (!isLetter(hunt)) {
                        cerr << "Multiple \'" << hunt << "\' found!" << endl;
                        exit(1);
                    }
                }
            }
        }
    }
    State findChar(char hunt) {
        State found;
        found.col = -1;
        found.row = -1;
        found.color = '\0';
        locateInMap(hunt, found);
        if (found.col == -1) {
            cerr << "No \'" << hunt << "\' symbol was found!!" << endl;
            exit(1);
        }
        return found;
    }
    char char2CapChar(char little) {
        return static_cast<char>(little - 32);
    }
    void findInvalidChar() {
        State huntedCapital;
        State hunted;
        huntedCapital.col = -1;
        hunted.col = -1;
        for (int i = colors + 1; i < 27; ++i) {
            locateInMap(int2Char(i), hunted);
            locateInMap(char2CapChar(int2Char(i)), huntedCapital);
            if (hunted.col != -1 || huntedCapital.col != -1) {
                cerr << "Invalid char \'" << int2Char(i) << "\' or '" 
                    << char2CapChar(int2Char(i)) << "\'" << endl;
                exit(1);
            }
        }
    }
    State findStart() {
        return findChar('@');
    }
    State findEnd() {
        return findChar('?');
    }
    bool isValidCharacter(char current) {
        if (current == '.' || current == '?' || current == '#' || 
            current == '^' || current == '@' || isLetter(current)) {
            return true;
        }
        return false;
    }
    void checkMap() {
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                if (!isValidCharacter(puzzle[i][j])) {
                    cerr << "Invalid char \'" << puzzle[i][j] << "\'" << endl;
                    exit(1);
                }
            }
        }
    }
    bool checkNorth(State current) {
        if (current.row == 0) {
            return false;
        }
        char north = puzzle[current.row - 1][current.col];
        int northInt = char2Int(north);
        if (!isValidCharacter(north)) {
            return false;
        }
        if (((northInt >= 0 && northInt < 27) && (northInt - 32 != char2Int(current.color))) ||
            ((northInt > -32 && northInt < -5) && (northInt + 32 == char2Int(current.color)))) {
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
        if (current.col == (int)width - 1) {
            return false;
        }
        char east = puzzle[current.row][current.col + 1];
        int eastInt = char2Int(east);
        if (!isValidCharacter(east)) {
            return false;
        }
        if (((eastInt >= 0 && eastInt < 27) && (eastInt - 32 != char2Int(current.color))) ||
            ((eastInt > -32 && eastInt < -5) && (eastInt + 32 == char2Int(current.color)))) {
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
        if (current.row == (int)height - 1) {
            return false;
        }
        char south = puzzle[current.row + 1][current.col];
        int southInt = char2Int(south);
        if (!isValidCharacter(south)) {
            return false;
        }
        if (((southInt >= 0 && southInt < 27) && (southInt - 32 != char2Int(current.color))) ||
            ((southInt > -32 && southInt < -5) && (southInt + 32 == char2Int(current.color)))) {
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
        int westInt = char2Int(west);
        if (!isValidCharacter(west)) {
            return false;
        }
        if (((westInt >= 0 && westInt < 27) && (westInt - 32 != char2Int(current.color))) ||
            ((westInt > -32 && westInt < -5) && (westInt + 32 == char2Int(current.color)))) {
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
                finalLocation = temp;
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
                finalLocation = temp;
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
                finalLocation = temp;
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
                finalLocation = temp;
                return;
            }
        }
    }
    bool isButton(State current) {
        if (((isLetter(puzzle[current.row][current.col]) && 
            !isCapital(puzzle[current.row][current.col])) || 
            puzzle[current.row][current.col] == '^') &&
            puzzle[current.row][current.col] != current.color) {
            return true;
        }
        return false;
    }
    bool isButtonBacktrack(State current) {
        if ((isLetter(puzzle[current.row][current.col]) &&
            !isCapital(puzzle[current.row][current.col])) ||
            puzzle[current.row][current.col] == '^') {
            return true;
        }
        return false;
    }
    bool findInReachable(State current) {
        for (unsigned int i = 0; i < reachable.size(); ++i) {
            if (reachable[i].col == current.col &&
                reachable[i].row == current.row &&
                reachable[i].color == current.color) {
                return true;
            }
            //return false;
        }
        return false;
    }
    bool isReached(int row, int col) {
        for (unsigned int i = 0; i < colors + 1; ++i) {
            if (backtrack[i][row][col] != '.') {
                return true;
            }
        }
        return false;
    }
    void printReachable() {
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                if (isReached(i, j)) {
                    cout << puzzle[i][j];
                }
                else {
                    cout << '#';
                }
            }
            cout << endl;
        }
        
    }
    bool isCapital(char letter) {
        return (letter >= 'A' && letter <= 'Z');
    }
    void solutionFinder() {
        State current = findEnd();
        current = findStart();
        beginLocation = current;
        State tempState;
        reachable.push_back(current);
        backtrack[char2Int('^')][current.row][current.col] = '@';
        findInvalidChar();
        checkMap();
        State maybeButton;
        while (!reachable.empty()) {
            if (!end) {
                /*if (backtrack[3][16][1] != '.') {
                    cout << "ope";
                }*/
                
                if (isQueue) {
                    current = reachable.front();
                    reachable.pop_front();
                }
                else {
                    current = reachable.back();
                    reachable.pop_back();
                }
                
                /*if (current.row == 17 && current.col == 1 && current.color == 'c') {
                    cout << "ope";
                }*/
                maybeButton.color = puzzle[current.row][current.col];
                maybeButton.row = current.row;
                maybeButton.col = current.col;
                if (isButton(current) && !findInReachable(maybeButton) && 
                    backtrack[char2Int(maybeButton.color)][maybeButton.row][maybeButton.col] == '.') {
                    tempState.color = puzzle[current.row][current.col];
                    tempState.col = current.col;
                    tempState.row = current.row;
                    reachable.push_back(tempState);
                    backtrack[char2Int(tempState.color)][current.row][current.col] 
                        = current.color;
                }
                else if (!isButton(current)) {
                    checkDirectionAndPush(current);
                }
            }
            else {
                current = reachable.back();
                reachable.clear();
            }
        }
        if (!end) {
            cout << "No solution." << endl << "Reachable:" << endl;
            printReachable();
            exit(0);
        }
        // do stufff
        
    }
    ///////////////////////////////////////////////////////////////////////////
    bool statesEqual(State lhs, State rhs) {
        if (lhs.col == rhs.col &&
            lhs.row == rhs.row &&
            lhs.color == rhs.color) {
            return true;
        }
        return false;
    }
    void printState(State ope) {
        cout << "(" << ope.color << ", (" << ope.row << ", " << ope.col << 
        "))" << endl;
    }
    void replaceSpot(State spot, bool yeet) {
        if (yeet) {
            backtrack[char2Int(spot.color)][spot.row][spot.col]
                = '@';
            return;
        }
        if (isButtonBacktrack(spot)) {
            if (puzzle[spot.row][spot.col] == spot.color) {
                backtrack[char2Int(spot.color)][spot.row][spot.col] 
                    = '+';
                return;
            }
            backtrack[char2Int(spot.color)][spot.row][spot.col] = '%';
            return;
        }
        if (puzzle[spot.row][spot.col] == '@' && spot.color == '^') {
            backtrack[char2Int(spot.color)][spot.row][spot.col] = '@';
            return;
        }
        if (puzzle[spot.row][spot.col] == '?') {
            backtrack[char2Int(spot.color)][spot.row][spot.col]
                = '?';
            return;
        }
        backtrack[char2Int(spot.color)][spot.row][spot.col] = '+';
    }
    void fillSolutionVector() {
        State current = finalLocation;
        State finish = findStart();
        char nextColor = '\0';
        while (!statesEqual(current, finish)) {
            //current.color = lastColor;
            char checker = backtrack[char2Int(current.color)][current.row][current.col];
            answer.push_back(current);
            if (checker == 'N') {
                replaceSpot(current, false);
                current.row -= 1;
            }
            else if (checker == 'E') {
                replaceSpot(current, false);
                current.col += 1;
            }
            else if (checker == 'S') {
                replaceSpot(current, false);
                current.row += 1;
            }
            else if (checker == 'W') {
                replaceSpot(current, false);
                current.col -= 1;
            }
            else {
                nextColor = backtrack[char2Int(current.color)][current.row][current.col];
                replaceSpot(current, true);
                current.color = nextColor;
            }
        }
        answer.push_back(beginLocation);
    }
    void printListOutput() {
        for (size_t i = answer.size() - 1; i > 0; --i) {
            printState(answer[i]);
        }
        printState(finalLocation);
    }
    bool isMatchingButton(State spot) {
        if (spot.color - 32 == puzzle[spot.row][spot.col]) {
            return true;
        }
        return false;
    }
    void printCharacter(State spot, bool isButton) {
        char puzzSpot = puzzle[spot.row][spot.col];
        char backSpot = backtrack[char2Int(spot.color)][spot.row][spot.col];
        if (backSpot == '@' || backSpot == '+' || backSpot == '%') {
            cout << backSpot;
        }
        else if (isButton) {
            if (spot.color == puzzSpot) {
                cout << '.';
            }
            else {
                cout << puzzSpot;
            }
        }
        else if ((spot.row == beginLocation.row &&
            spot.col == beginLocation.col)) {
            cout << '.';
        }
        else if (isMatchingButton(spot)) {
            cout << '.';
        }
        else if (puzzSpot == '@' && spot.color != '^') {
            cout << '.';
        }
        else {
            cout << puzzSpot;
        } // else
    }
    void printAlteredMap(int color) {
        bool buttonBool;
        State current;
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                current.color = int2Char(color);
                current.row = i;
                current.col = j;
                buttonBool = isButtonBacktrack(current);
                printCharacter(current, buttonBool);
            }
            cout << endl;
        }
    }
    void printMapOutput() {
        for (unsigned int i = 0; i < colors + 1; ++i) {
            cout << "// color " << int2Char(i) << endl;
            printAlteredMap(i);
        } // for i
    } // printMapOutput()
    void writeOutput() {
        if (type == "list") {
            fillSolutionVector();
            printListOutput();
        }// if
        else {
            fillSolutionVector();
            printMapOutput();
        } // else
    } // writeOutput()
    ///////////////////////////////////////////////////////////////////////////
};