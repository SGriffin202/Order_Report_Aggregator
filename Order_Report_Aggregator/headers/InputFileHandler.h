#ifndef INPUTFILEHANDLER_H
#define INPUTFILEHANDLER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class InputFileHandler
{
protected:
    std::string inputFile;

    virtual void ReadInputData(const std::string& inputLine) = 0;
    std::vector<std::string> SplitString(const std::string& originalString, char delimiter);

public:
    InputFileHandler(const std::string& inputFile_);
    ~InputFileHandler();
    void SetInputFile(const std::string& inputFile_);
    void ReadInputFile();
};

#endif