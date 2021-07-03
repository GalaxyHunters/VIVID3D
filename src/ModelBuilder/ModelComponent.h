#ifndef VIVID_MODELCOMPONENT_H
#define VIVID_MODELCOMPONENT_H

#include <string>

namespace vivid
{

/* Abstract Class defining 3D component for a model. Can be mesh, line, point cloud or anything else. */
class CModelComponent {
    std::string mLabel = "";
};

}; // namespace vivid
#endif //VIVID_MODELCOMPONENT_H
