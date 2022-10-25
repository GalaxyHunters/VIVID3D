#include "BlobData.h"
#include "LogFile.h"

using namespace std;
using namespace vivid;

CBlobData CBlobData::FormatExportDataBlob(const aiExportDataBlob *apBlob) {
    auto blob = apBlob;

    vector<string> names;
    vector<string> files;

    while (blob->data) {
        names.emplace_back(blob->name.C_Str());
        string file(static_cast<char*>(blob->data), blob->size);

        files.push_back(file);
        if (!blob->next) {
            break;
        }
        blob = blob->next;
    }
    return CBlobData(names.size(), names, files);
}
