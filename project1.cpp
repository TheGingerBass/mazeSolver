// Project Identifier: A8A3A33EF075ACEF9B08F5B9845569ECCB423725
// Noah Bonello

#include "Solution.h"

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    Solution s;
    string mode = s.getoptPrep(argc, argv);
    s.readInput();
    s.print2DVector();
    s.solutionFinder();
    //s.writeOutput();
    return 0;
}

