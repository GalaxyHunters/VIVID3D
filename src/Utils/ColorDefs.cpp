#include "ColorDefs.h"

std::vector<color_t> GetColorByString(const std::string &arCName) {
    std::map<std::string, FColor_t> ColorFunc{{"white",  White},
                                              {"black",  Black},
                                              {"red",    Red},
                                              {"green",  Green},
                                              {"blue",   Blue},
                                              {"yellow", Yellow},
                                              {"orange", Orange},
                                              {"purple", Purple},
                                              {"cyan",   Cyan},
    };
    return ColorFunc[arCName]();
}