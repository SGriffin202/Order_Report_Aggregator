#include <OutputFileHandler.h>

OutputFileHandler::OutputFileHandler(const std::string& outputFile_)
    : outputFile(outputFile_)
{
}

OutputFileHandler::~OutputFileHandler()
{
}

void OutputFileHandler::SetOutputFile(const std::string& outputFile_)
{
    outputFile = outputFile_;
}

void OutputFileHandler::WriteOutputFile()
{
    std::ofstream outStream(outputFile);

    WriteOutputData(outStream);

    outStream.close();
}