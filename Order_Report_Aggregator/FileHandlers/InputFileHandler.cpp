#include "InputFileHandler.h"

InputFileHandler::InputFileHandler(const std::string& inputFile_)
    : inputFile(inputFile_)
{
}

InputFileHandler::~InputFileHandler()
{
}

void InputFileHandler::SetInputFile(const std::string& inputFile_)
{
    inputFile = inputFile_;
}

std::string InputFileHandler::GetInputFile()
{
    return inputFile;
}

void InputFileHandler::ReadInputFile()
{
    std::ifstream stream(inputFile);
    std::string line;

    while(std::getline(stream, line))
    {
        ReadInputData(line);
    }

    stream.close();
}

std::vector<std::string> InputFileHandler::SplitString(const std::string& originalString, char delimiter)
{
    std::vector<std::string> splitStrings{};
    std::istringstream ss(originalString);
    std::string splitString;

    while(std::getline(ss, splitString, delimiter))
        splitStrings.push_back(splitString);

    return splitStrings;
}