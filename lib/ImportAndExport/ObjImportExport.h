#ifndef VIVID_OBJIMPORTEXPORT_H
#define VIVID_OBJIMPORTEXPORT_H

#include "Model.h"


namespace vivid
{
//TODO: THIS SHOULD REALLLY NOT BE EMPTY!!!
//output functions
//TODO should habe return value, and maybe exception!
void OBJExporter(CModel &aModel, std::string aOutPutFile, bool WithTexture = true); //=1

//CModel ImportToObj(ostream* aInPutFile);

} // namespace vivid
#endif //VIVID_OBJIMPORTEXPORT_H
