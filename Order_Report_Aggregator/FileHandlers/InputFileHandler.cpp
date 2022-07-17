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


/** @brief Calculate the position of a string value, associated with a heading, from an input string
 * 
 *  @param searchStr - String containing the value that needs to be extracted
 *  @param heading   - Heading in the searchStr for the value that needs to be extracted
 *  @param valPos    - Position of the string value that needs to be extracted
 *  @param valLength - Length of the string value that needs to be extracted
 *  @return void
 */
void InputFileHandler::CalcStrValPosFromStr( const std::string& searchStr,
                                             const std::string& heading,
                                             size_t&            valPos,
                                             size_t&            valLength ) const
{
    size_t startPos = 0;
    size_t endPos = 0;
    valPos = 0;
    valLength = 0;

    startPos = searchStr.find(heading);
    if (startPos != std::string::npos)
    {
        endPos = searchStr.find(',', startPos);
        int headingLen = heading.size();
        valPos = startPos + headingLen;
        valLength = endPos - valPos;
    }
}