/** @file InputFileHandler.cpp
 *  @brief Base class to handle reading in input files
 *
 *  Base class for reading in an input file. Will read in the input file line by line,
 *  sending sending each line to the ReadInputData virtual function. This function
 *  should be overridden in the derived class so that each line can be processed.
 *  
 *  Also contains helper function(s) that may be useful when manipulating the input data.
 *
 *  @author Sean Griffin
 *  @bug No known bugs.
 */

#include "InputFileHandler.h"

/** @brief Input File Handler Constructor
 *
 *  @param inputFile_ - Input File Name/Path
 */
InputFileHandler::InputFileHandler(const std::string& inputFile_)
    : inputFile(inputFile_)
{
}

InputFileHandler::~InputFileHandler()
{
}


/** @brief Sets the Input File
 * 
 *  @param inputFile_ - Input File Name/Path
 *  @return void
 */
void InputFileHandler::SetInputFile(const std::string& inputFile_)
{
    inputFile = inputFile_;
}


/** @brief Reads in the Input File
 * 
 *  Will read in the Input File line by line, sending each line to the
 *  ReadInputData virtual function.
 * 
 *  @return void
 */
void InputFileHandler::ReadInputFile()
{
    std::ifstream stream(inputFile);
    std::string line;

    while(std::getline(stream, line))
        ReadInputData(line);

    stream.close();
}


/** @brief Splits the provided string by the specified delimiter
 *
 *  @param originalString - The string to split
 *  @param delimiter      - The delimiter to split the string
 *  @return The split string
 */
std::vector<std::string> InputFileHandler::SplitString(const std::string& originalString, char delimiter)
{
    std::vector<std::string> splitStrings{};
    std::istringstream ss(originalString);
    std::string splitString;

    while(std::getline(ss, splitString, delimiter))
        splitStrings.push_back(splitString);

    return splitStrings;
}