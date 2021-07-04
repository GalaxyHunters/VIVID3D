#include "DataToImage.h"
#include "lodepng.h"

#include <iostream> //TODO DELETE, USE LOG INSTEAD

void vivid::encodePNG(const std::string &arFilename, std::vector<unsigned char> &arIn, unsigned int aWidth, unsigned int aHeight) {
    auto err_code = lodepng::encode(arFilename, arIn, aWidth, aHeight);

    if(err_code) { //TODO check err_code value! bad code!
        std::cout << "encoder err_code " << err_code << ": " << lodepng_error_text(err_code) << std::endl;
    }
}