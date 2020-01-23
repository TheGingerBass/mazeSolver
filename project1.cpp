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

