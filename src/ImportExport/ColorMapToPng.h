#ifndef VIVID_COLORMAPTOPNG_H
#define VIVID_COLORMAPTOPNG_H
#include <string>
#include <vector>


namespace vivid
{

struct CMemoryBuffer {
    const unsigned char* mBuffer;
    const size_t mSize;

    CMemoryBuffer(const unsigned char* apBuffer, const size_t aSize) : mBuffer(apBuffer), mSize(aSize) {}
};

void encodePNG(const std::string &arFilename, std::vector<unsigned char> &arIn, unsigned int aWidth, unsigned int aHeight);
const CMemoryBuffer encodePNG(std::vector<unsigned char> &arIn, unsigned int aWidth, unsigned int aHeight);

}; // namespace vivid
#endif //VIVID_COLORMAPTOPNG_H

