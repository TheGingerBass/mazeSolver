// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725
// Noah Bonello

#include <string>
#include <iostream>
#include <deque>
#include <vector>
#include <stack>
using namespace std;

struct State {
    char color;
    int row;
    int col;
};

class Solution {
private:
    string solution = "";
    unsigned int colors = 0;
    unsigned int height = 0;
    unsigned int width = 0;
    bool start = false;
    bool end = false;
    vector < vector<char>> puzzle;
    vector < vector < vector <char>>> backtrack;
public:
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
    void solutionFinder() {
        deque<State> deck;
        State current = findStart();
        deck.push_back(current);
        current = deck.front();
        deck.pop_front();
        State temp;
        if (checkNorth(current)) {
            temp.col = current.col;
            temp.row = current.row - 1;
            temp.color = current.color;
            deck.push_back(temp);
        }
    }
    void writeOutput();
};