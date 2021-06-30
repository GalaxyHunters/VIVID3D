#ifndef VIVID_DATATOIMAGE_H
#define VIVID_DATATOIMAGE_H
#include <string>
#include <vector>


namespace vivid
{

void encodePNG(const std::string &arFilename, std::vector<unsigned char> &arIn, unsigned int aWidth, unsigned int aHeight);


}; // namespace vivid
#endif //VIVID_DATATOIMAGE_H

