#ifndef VIVID_PROGRESSBAR_H
#define VIVID_PROGRESSBAR_H

#include <string>
#include <iostream>

using namespace std;

namespace vivid {
    /**
    * Utility function for tracking loop progress in console
    * @param[in] percentage normalized value from 0.0-1.0, should be (double)index/array.size()
    */
    void printProgress(double percentage);
}
#endif //VIVID_PROGRESSBAR_H