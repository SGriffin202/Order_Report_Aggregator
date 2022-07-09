#include "OrderReportFileHandler.h"

OrderReportFileHandler::OrderReportFileHandler( const std::string& inputFile_,
                                                const std::string& outputFile_,
                                                OrderReportCollection& ordRptColl_,
                                                bool rptEmptyOrds_ )
    : InputFileHandler(inputFile_),
      OutputFileHandler(outputFile_),
      ordRptColl(std::move(ordRptColl_)),
      reportEmptyOrders(rptEmptyOrds_)
{
}

OrderReportFileHandler::~OrderReportFileHandler()
{
}

void OrderReportFileHandler::SetReportEmptyOrders(bool rptEmptyOrds)
{
    reportEmptyOrders = rptEmptyOrds;
}

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

// This function is creating a OrderAddData object to temporary store the values from the Order Add ("msgType_":12).
// It is then searching the ordRptColl map by the securityId to see if an OrderReport object exists.
// If it finds an OrderReport object then it adds the data from the OrderAddData object to the OrderReport object.
//
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

// This function first creates a new empty OrderReport object.
// It then stores the securityId_, isin_ & currency_ from the
// Security Reference Data ("msgType_":8) into the OrderReport object.
// Finally it then adds the OrderReport object to the ordRptColl map.
//
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

// Write the Order Report
// ISIN | Currency | Total Buy Count | Total Sell Count | Total Buy Quantity | Total Sell Quantity |
// Weighted Average Buy Price | Weighted Average Sell Price | Max Buy Price | Min Sell Price
//
void OrderReportFileHandler::WriteOutputData(std::ofstream &outStream)
{
    char delim = '\t';
    outStream << "ISIN" << delim
              << "Currency" << delim
              << "Total Buy Count" << delim
              << "Total Sell Count" << delim
              << "Total Buy Quantity" << delim
              << "Total Sell Quantity" << delim
              << "Weighted Average Buy Price" << delim
              << "Weighted Average Sell Price" << delim
              << "Max Buy Price" << delim
              << "Min Sell Price"
              << "\n";

    for(auto& ord : ordRptColl)
        outStream << ord.second->OutputReport(delim, reportEmptyOrders);
}