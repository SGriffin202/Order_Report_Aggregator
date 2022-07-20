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
                                                std::shared_ptr<OrderReportCollection> ordRptColl_,
                                                const char delim,
                                                const bool rptEmptyOrds_ )
    : InputFileHandler(inputFile_),
      OutputFileHandler(outputFile_),
      ordRptColl(ordRptColl_),
      outputFileDelimiter(delim),
      reportEmptyOrders(rptEmptyOrds_)
{
    if(ordRptColl == nullptr)
        ordRptColl = std::make_shared<OrderReportCollection>();
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
void OrderReportFileHandler::SetOutputFileDelimiter(const char delim)
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
void OrderReportFileHandler::SetReportEmptyOrders(const bool rptEmptyOrds)
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
    size_t valPos = 0;
    size_t valLength = 0;
    OrderAddData tmpData;
    
    // Message type 12 will always have the headings we need in the following order:
    // securityId_\":, side_\":, quantity_\":, price_\":
    // So rather than searching the whole string for these headings each time, we can begin our
    // search for each subsequent heading from where the value of the previous heading ended.
    //
    CalcStrValPosFromStr(inputLine, "securityId_\":", valPos, valLength);
    int securityId = stoi(inputLine.substr(valPos, valLength));

    CalcStrValPosFromStr(inputLine, "side_\":", valPos, valLength, (valPos + valLength));
    tmpData.side = (inputLine.substr(valPos, valLength) == "BUY") ? Side::Buy : Side::Sell;

    CalcStrValPosFromStr(inputLine, "quantity_\":", valPos, valLength, (valPos + valLength));
    tmpData.quantity = stoll(inputLine.substr(valPos, valLength));

    CalcStrValPosFromStr(inputLine, "price_\":", valPos, valLength, (valPos + valLength));
    tmpData.price = stoll(inputLine.substr(valPos, valLength));

    auto find = ordRptColl->find(securityId);
    if ( find != ordRptColl->end() )
        find->second.AddOrderData(tmpData);
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
    size_t valPos = 0;
    size_t valLength = 0;
    OrderReport newOrdRpt = OrderReport();

    // Message type 8 will always have the headings we need in the following order:
    // securityId_\":, isin_\":, currency_\":
    // So rather than searching the whole string for these headings each time, we can begin our
    // search for each subsequent heading from where the value of the previous heading ended.
    //
    CalcStrValPosFromStr(inputLine, "securityId_\":", valPos, valLength);
    newOrdRpt.SetSecurityId(stoi(inputLine.substr(valPos, valLength)));

    CalcStrValPosFromStr(inputLine, "isin_\":", valPos, valLength, (valPos + valLength));
    newOrdRpt.SetISIN(inputLine.substr(valPos, valLength));

    CalcStrValPosFromStr(inputLine, "currency_\":", valPos, valLength, (valPos + valLength));
    newOrdRpt.SetCurrency(inputLine.substr(valPos, valLength));

    ordRptColl->insert(std::make_pair(newOrdRpt.GetSecurityId(), std::move(newOrdRpt)));
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
void OrderReportFileHandler::WriteOutputData(std::ofstream& outStream) const
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

    for(auto ord : *ordRptColl)
        ord.second.OutputReport(outStream, outputFileDelimiter, reportEmptyOrders);
}