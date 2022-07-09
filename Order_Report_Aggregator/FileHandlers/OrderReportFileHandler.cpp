/** @file OrderReportFileHandler.cpp
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

#include "OrderReportFileHandler.h"

/** @brief Order Report File Handler Constructor
 *
 *  @param inputFile_    - Input File Name/Path
 *  @param outputFile_   - Output File Name/Path
 *  @param ordRptColl_   - Map of Order Report objects
 *  @param delim         - Output File Delimiter
 *  @param rptEmptyOrds_ - Denotes whether to output Securities with no Orders against them
 */
OrderReportFileHandler::OrderReportFileHandler( const std::string& inputFile_,
                                                const std::string& outputFile_,
                                                OrderReportCollection& ordRptColl_,
                                                char delim,
                                                bool rptEmptyOrds_ )
    : InputFileHandler(inputFile_),
      OutputFileHandler(outputFile_),
      ordRptColl(std::move(ordRptColl_)),
      outputFileDelimiter(delim),
      reportEmptyOrders(rptEmptyOrds_)
{
}

OrderReportFileHandler::~OrderReportFileHandler()
{
}


/** @brief Sets the Output File Delimiter
 * 
 *  The delimiter between each heading/data value.
 * 
 *  @param delim - Output File Delimiter
 *  @return void
 */
void OrderReportFileHandler::SetOutputFileDelimiter(char delim)
{
    outputFileDelimiter = delim;
}


/** @brief Sets Report Empty Orders
 * 
 *  This will denote whether to output Securities with no Orders against them
 *  to the Order Report file.
 * 
 *  @param rptEmptyOrds - Report Empty Orders
 *  @return void
 */
void OrderReportFileHandler::SetReportEmptyOrders(bool rptEmptyOrds)
{
    reportEmptyOrders = rptEmptyOrds;
}


/** @brief Reads the line from the input file
 * 
 *  Will check if the line from the input file contains "msgType_":8" or "msgType_":12".
 *  It will send the line to the appropiate function depending on the message type.
 *
 *  @param inputLine - Line from the input file
 *  @return void
 */
void OrderReportFileHandler::ReadInputData(const std::string& inputLine)
{
    // There are a lot more MSG_TYPE_ORDER_ADD than MSG_TYPE_SECURITY_REF
    // so we should evaluate it first
    //
    if (inputLine.find(MSG_TYPE_ORDER_ADD) != std::string::npos)
    {
        // Find existing Order object and modify it
        //
        FindAndUpdateOrderReport(inputLine);
    }
    else if (inputLine.find(MSG_TYPE_SECURITY_REF) != std::string::npos)
    {
        // Add new Order object
        //
        CreateOrderReport(inputLine);
    }
}


/** @brief Find and update an Order Report object
 * 
 *  This function is creating a OrderAddData object to temporary store the values from the Order Add ("msgType_":12).
 *  It is then searching the ordRptColl map by the securityId to see if an OrderReport object exists.
 *  If it finds an OrderReport object then it adds the data from the OrderAddData object to the OrderReport object.
 *
 *  @param inputLine - Line from the input file that contains the Order Add record (msgType_ = 12)
 *  @return void
 */
void OrderReportFileHandler::FindAndUpdateOrderReport(const std::string& inputLine)
{
    OrderAddData tmpData;
    std::vector<std::string> headingsValues = SplitString(inputLine, ',');
    for (std::string headingValue : headingsValues)
    {
        std::vector<std::string> headValSplit = SplitString(headingValue, ':');
        if (headValSplit[0] == "\"bookEntry_\"") // The headingValue for this is: "bookEntry_":{"securityId_":VALUE
            tmpData.securityId = stoi(headValSplit[2]);
        else if (headValSplit[0] == "\"side_\"")
            tmpData.side = (headValSplit[1] == "BUY") ? 'B' : 'S';
        else if (headValSplit[0] == "\"quantity_\"")
            tmpData.quantity = stoll(headValSplit[1]);
        else if (headValSplit[0] == "\"price_\"")
            tmpData.price = stoll(headValSplit[1]);
    }

    if ( ordRptColl.find(tmpData.securityId) != ordRptColl.end() )
        ordRptColl[tmpData.securityId]->AddOrderData(tmpData);
}


/** @brief Creates an Order Report object from an Order Add record
 * 
 *  This function first creates a new empty OrderReport object.
 *  It then stores the securityId_, isin_ & currency_ from the
 *  Security Reference Data ("msgType_":8) into the OrderReport object.
 *  Finally it then adds the OrderReport object to the ordRptColl map.
 *
 *  @param inputLine - Line from the input file that contains the Security Reference Data record (msgType_ = 8)
 *  @return void
 */
void OrderReportFileHandler::CreateOrderReport(const std::string& inputLine) 
{
    std::unique_ptr<OrderReport> newOrdRpt = std::make_unique<OrderReport>();

    std::vector<std::string> headingsValues = SplitString(inputLine, ',');
    for (std::string headingValue : headingsValues)
    {
        std::vector<std::string> headValSplit = SplitString(headingValue, ':');
        if (headValSplit[0] == "\"security_\"") // headingValue = "security_":{"securityId_":VALUE
            newOrdRpt->SetSecurityId(stoi(headValSplit[2]));
        else if (headValSplit[0] == "\"isin_\"")
            newOrdRpt->SetISIN(headValSplit[1]);
        else if (headValSplit[0] == "\"currency_\"")
            newOrdRpt->SetCurrency(headValSplit[1]);
    }

    ordRptColl.insert(std::make_pair(newOrdRpt->GetSecurityId(), std::move(newOrdRpt)));
}


/** @brief Outputs the data needed to the Order Report File
 * 
 *  The output report will contain the following headers in the following order:
 *    ISIN | Currency | Total Buy Count | Total Sell Count | Total Buy Quantity | Total Sell Quantity |
 *    Weighted Average Buy Price | Weighted Average Sell Price | Max Buy Price | Min Sell Price
 * 
 *  It will go through the collection of Order Reports and write the output each Order Report object
 *  to the output (Order Report) file.
 *
 *  @param outStream - The stream to the Order Report file
 *  @return void
 */
void OrderReportFileHandler::WriteOutputData(std::ofstream& outStream)
{
    outStream << "ISIN" << outputFileDelimiter
              << "Currency" << outputFileDelimiter
              << "Total Buy Count" << outputFileDelimiter
              << "Total Sell Count" << outputFileDelimiter
              << "Total Buy Quantity" << outputFileDelimiter
              << "Total Sell Quantity" << outputFileDelimiter
              << "Weighted Average Buy Price" << outputFileDelimiter
              << "Weighted Average Sell Price" << outputFileDelimiter
              << "Max Buy Price" << outputFileDelimiter
              << "Min Sell Price"
              << "\n";

    for(auto& ord : ordRptColl)
        outStream << ord.second->OutputReport(outputFileDelimiter, reportEmptyOrders);
}