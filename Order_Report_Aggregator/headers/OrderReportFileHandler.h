/** @file OrderReportFileHandler.h
 *  @brief Reads in and outputs data for an Order Report on Securities
 *
 *  This contains the data and functions needed to produce an Order Report on a collection of Securities.
 *  
 *  It will read the input file one line at a time and search for the strings "msgType_":8" & "msgType_":12".
 *
 *  It creates an Order Report object and inserts it into the map for Message Type 8. For Message Type 12 it will
 *  search the map, and if it finds a Security ID that matches then it will update the related Order Report object.
 * 
 *  When outputing the Order Report File it will loop through every Order Report object in the Order Report map,
 *  outputting the required data in the specified format.
 *
 *  @author Sean Griffin
 *  @bug No known bugs.
 */

#ifndef ORDERREPORTFILEHANDLER_H
#define ORDERREPORTFILEHANDLER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "InputFileHandler.h"
#include "OutputFileHandler.h"
#include "OrderReport.h"

typedef std::unordered_map<int, std::unique_ptr<OrderReport>> OrderReportCollection;

class OrderReportFileHandler : public InputFileHandler, public OutputFileHandler
{
private:
    const std::string MSG_TYPE_SECURITY_REF = "msgType_\":8";
    const std::string MSG_TYPE_ORDER_ADD = "msgType_\":12";
    OrderReportCollection* ordRptColl;
    char outputFileDelimiter;
    bool reportEmptyOrders;

    void ReadInputData(const std::string& inputLine) override;
    void FindAndUpdateOrderReport(const std::string& inputLine);
    void CreateOrderReport(const std::string& inputLine);
    void WriteOutputData(std::ofstream& outStream) override;

public:
    OrderReportFileHandler( const std::string& inputFile_,
                            const std::string& outputFile_,
                            OrderReportCollection* ordRptColl_,
                            char delim,
                            bool rptEmptyOrds_ );
    ~OrderReportFileHandler();
    void SetOutputFileDelimiter(char delim);
    void SetReportEmptyOrders(bool rptEmptyOrds);
};

#endif