#ifndef VIVID_BLOBDATA_H
#define VIVID_BLOBDATA_H

#include "assimp/cexport.h"
#include <vector>
#include <string>

namespace vivid {
    struct CBlobData {
        const size_t mNumFiles;
        const std::vector<std::string> mNames;
        // Why?! Because we fucking can!!! And if we can, we do!!!
        const std::vector<std::string> mFiles;

        CBlobData() : mNumFiles(), mNames(), mFiles() {}
        CBlobData(const size_t aNumFiles, const std::vector<std::string>& arNames, const std::vector<std::string>& arFiles)
            : mNumFiles(aNumFiles), mNames(arNames), mFiles(arFiles) { }

        ~CBlobData() = default;

        static CBlobData FormatExportDataBlob(const aiExportDataBlob* apBlob);
    };

}; // namespace vivid
#endif //VIVID_BLOBDATA_H
