/** @file InputFileHandler.h
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

#ifndef INPUTFILEHANDLER_H
#define INPUTFILEHANDLER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class InputFileHandler
{
protected:
    std::string inputFile;

    virtual void ReadInputData(const std::string& inputLine) = 0;
    void CalcStrValPosFromStr( const std::string& searchStr,
                               const std::string& heading,
                               size_t&            valPos,
                               size_t&            valLength ) const;

public:
    InputFileHandler(const std::string& inputFile_);
    virtual ~InputFileHandler();
    void SetInputFile(const std::string& inputFile_);
    void ReadInputFile();
};

#endif