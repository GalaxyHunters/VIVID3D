#include "ColorMapToPng.h"
#include "lodepng.h"
#include "LogFile.h"

using namespace std;

namespace vivid {
    void encodePNG(const string &arFilename, vector<unsigned char> &arIn, unsigned int aWidth,
                   unsigned int aHeight) {
        auto err_code = lodepng::encode(arFilename, arIn, aWidth, aHeight);

        if (err_code) {
            Log(LOG_ERROR, lodepng_error_text(err_code));
        }
    }

    const CMemoryBuffer encodePNG(vector<unsigned char> &arIn, unsigned int aWidth, unsigned int aHeight) {
        unsigned char *buffer;
        size_t buffersize;
        auto err_code = lodepng::encode(&buffer, &buffersize, arIn, aWidth, aHeight);

        if (err_code) {
            Log(LOG_ERROR, lodepng_error_text(err_code));
        }
        return {buffer, buffersize};
    }
} // namespace vivid