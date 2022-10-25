#include "BlobData.h"
#include "LogFile.h"

using namespace std;
using namespace vivid;

const size_t CBlobData::getNumFiles() const {
    size_t count = 0;
    auto blob = mBlob;
    while (blob->next) {
        count++;
        if (!blob->next) {
            break;
        }
        blob = blob->next;
    }
    return count;
}
const std::vector<std::string> CBlobData::getNames() const {
    auto blob = mBlob;
    std::vector<std::string> names;
    while (blob->data) {
        names.emplace_back(blob->name.C_Str());
        if (!blob->next) {
            break;
        }
        blob = blob->next;
    }
    return names;
};
const std::vector<char*> CBlobData::getFiles() const {
    auto blob = mBlob;
    std::vector<char*> files;
    while (blob->data) {
        files.push_back(static_cast<char*>(blob->data));
        if (!blob->next) {
            break;
        }
        blob = blob->next;
    }
    return files;
};