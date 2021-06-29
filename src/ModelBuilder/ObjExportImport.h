#ifndef OBJEXPORTIMPORT_H
#define OBJEXPORTIMPORT_H

#include "Model.h"
#include "fstream"


using namespace std;
using namespace boost::algorithm;

//output functions
void ExportToObj(CModel *aModel, std::string aOutPutFile, bool WithTexture = 1);

//CModel ImportToObj(ostream* aInPutFile);

#endif
