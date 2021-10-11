#ifndef VIVID_PROGRESSBAR_H
#define VIVID_PROGRESSBAR_H

inline void printProgress(double percentage) {
    string PROG_LINES = "||||||||||||||||||||||||||||||||||||||||||||||||||";
    string PROG_SPACES = "                                                  ";
    int PROG_LENGTH = 50;
    if (percentage >= 0.999) {
        percentage = 1.0;
    }
    int val = (int) (percentage * 100);
    string str = "%  ["+ PROG_LINES.substr(0, val/(100/PROG_LENGTH)) + PROG_SPACES.substr(0, PROG_LENGTH-(val/(100/PROG_LENGTH))) + "]";
    cout << "\r" << val<<str;
}
#endif //VIVID_PROGRESSBAR_H