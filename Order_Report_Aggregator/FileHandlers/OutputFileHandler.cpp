/** @file OutputFileHandler.cpp
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

#include <OutputFileHandler.h>

/** @brief Output File Handler Constructor
 *
 *  @param outputFile_ - Output File Name/Path
 */
OutputFileHandler::OutputFileHandler(const std::string& outputFile_)
    : outputFile(outputFile_)
{
}

OutputFileHandler::~OutputFileHandler()
{
}


/** @brief Sets the Output File
 * 
 *  @param outputFile_ - Output File Name/Path
 *  @return void
 */
void OutputFileHandler::SetOutputFile(const std::string& outputFile_)
{
    outputFile = outputFile_;
}


/** @brief Writes to the Output File
 * 
 *  Will create a stream to the output file before passing the stream to the
 *  virtual function WriteOutputData, which should write the actual data that
 *  will go into the file.
 * 
 *  @return void
 */
void OutputFileHandler::WriteOutputFile()
{
    std::ofstream outStream(outputFile);

    WriteOutputData(outStream);

    outStream.close();
}