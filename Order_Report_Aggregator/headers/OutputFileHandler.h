/** @file OutputFileHandler.h
 *  @brief Base class to handle writing to output files
 *
 *  Base class for writing to an output file. Will create a stream to the output file
 *  before passing the stream to the virtual function WriteOutputData. This function 
 *  should be overridden in the derived class to write the actual data that will go
 *  into the file.
 *
 *  @author Sean Griffin
 *  @bug No known bugs.
 */

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
    virtual ~OutputFileHandler();
    void SetOutputFile(const std::string& outputFile_);
    void WriteOutputFile();
};

#endif