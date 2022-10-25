#ifndef VIVID_BLOBDATA_H
#define VIVID_BLOBDATA_H

#include "assimp/cexport.h"
#include <vector>

namespace vivid {
    class CBlobData {
    private:
        const aiExportDataBlob *mBlob;
    public:
        CBlobData(const aiExportDataBlob *apBlob) : mBlob(apBlob) {}

        ~CBlobData() {
            aiReleaseExportBlob(mBlob);
        }

        const size_t getNumFiles() const;

        const std::vector <std::string> getNames() const;

        const std::vector<char *> getFiles() const;
    };

}; // namespace vivid
#endif //VIVID_BLOBDATA_H
