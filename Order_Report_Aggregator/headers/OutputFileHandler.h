#ifndef OUTPUTFILEHANDLER_H
#define OUTPUTFILEHANDLER_H

#include <fstream>
#include <string>

class OutputFileHandler
{
protected:
    std::string outputFile;
    
    virtual void WriteOutputData(std::ofstream &outStream) = 0;

public:
    OutputFileHandler(const std::string& outputFile_);
    ~OutputFileHandler();
    void SetOutputFile(const std::string& outputFile_);
    void WriteOutputFile();
};

#endif