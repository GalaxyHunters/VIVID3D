#ifndef VIVID_PROGRESSBAR_H
#define VIVID_PROGRESSBAR_H
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress(double percentage) {
    if (percentage >= 0.999) {
        percentage = 1.0;
    }
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}
#endif //VIVID_PROGRESSBAR_H
