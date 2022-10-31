#ifndef VIVID_OBJIMPORTEXPORT_H
#define VIVID_OBJIMPORTEXPORT_H

#include "Model.h"


namespace vivid
{
void OBJExporter(CModel &aModel, std::string aOutPutFile, bool WithTexture = true);

//CModel ImportToObj(ostream* aInPutFile);

} // namespace vivid
#endif //VIVID_OBJIMPORTEXPORT_H
