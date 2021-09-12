#include "LogFile.h"

void SetLogFile(bool aWriteToLog, bool aPrintToConsole)
{
    CLogFile::GetInstance().SetWriteToLog(aWriteToLog);
    CLogFile::GetInstance().SetPrintToConsole(aPrintToConsole);
}
