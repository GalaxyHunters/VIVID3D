#include "ProgressBar.h"

using namespace std;

namespace vivid {
    namespace {
        int currProgress = -1;
    }

    void printProgress(double percentage) {
        string PROG_LINES  = "||||||||||||||||||||||||||||||||||||||||||||||||||";
        string PROG_SPACES = "                                                  ";
        int PROG_LENGTH = 50;
        if (percentage > 0.99) {
            percentage = 1.0;
        }
        int progress = (int) (percentage * 100);
        if (progress == currProgress) return;

        currProgress = progress;
        string str = "[" + PROG_LINES.substr(0, currProgress / (100 / PROG_LENGTH)) + PROG_SPACES.substr(0, PROG_LENGTH - (currProgress / (100 / PROG_LENGTH))) + "] ";
        cout << "\r" << str << currProgress << "%";
        if (currProgress == 100) {
            cout << endl;
        }
    }
}