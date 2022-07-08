#include "OrderFileHandler.h"

OrderFileHandler::OrderFileHandler(const std::string& inputFile_, const std::string& outputFile_)
    : inputFile(inputFile_), outputFile(outputFile_)
{
}

OrderFileHandler::~OrderFileHandler()
{
}

void OrderFileHandler::ReadInputFile(orderCollect& orderCollection)
{
    std::ifstream stream(inputFile);
    std::string line;

    const std::string MSG_TYPE_SECURITY_REF = "msgType_\":8";
    const std::string MSG_TYPE_ORDER_ADD = "msgType_\":12";
    const char DELIMITER = ':';
    size_t delimPos = 0;

    while(std::getline(stream, line))
    {
        // There are a lot more MSG_TYPE_ORDER_ADD than MSG_TYPE_SECURITY_REF
        // so we should evaluate it first
        //
        if (line.find(MSG_TYPE_ORDER_ADD) != std::string::npos)
        {
            // Find existing Order object and modify it
            //
            FindAndUpdateOrder(line, orderCollection);
        }
        else if (line.find(MSG_TYPE_SECURITY_REF) != std::string::npos)
        {
            // Add new Order object
            //
            CreateOrder(line, orderCollection);
        }
    }

    stream.close();
}

// This function is creating a OrderAddData object to temporary store the values from the Order Add ("msgType_":12).
// It is then searching the orderCollection map by the securityId to see if an Order object exists.
// If it finds an Order object then it adds the data from the OrderAddData object to the Order object.
//
void OrderFileHandler::FindAndUpdateOrder(std::string& inputLine, orderCollect& orderCollection)
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

    if ( orderCollection.find(tmpData.securityId) != orderCollection.end() )
        orderCollection[tmpData.securityId]->AddOrderData(tmpData);
}

// This function first creates a new empty Order object.
// It then stores the securityId_, isin_ & currency_ from the Security Reference Data ("msgType_":8) into the Order object.
// Finally it then adds the Order object to the orderCollection map.
//
void OrderFileHandler::CreateOrder(const std::string& inputLine, orderCollect& orderCollection)
{
    std::unique_ptr<Order> newOrder = std::make_unique<Order>();
    std::vector<std::string> headingsValues = SplitString(inputLine, ',');
    for (std::string headingValue : headingsValues)
    {
        std::vector<std::string> headValSplit = SplitString(headingValue, ':');
        if (headValSplit[0] == "\"security_\"") // The headingValue for this is: "security_":{"securityId_":VALUE
            newOrder->SetSecurityId(stoi(headValSplit[2]));
        else if (headValSplit[0] == "\"isin_\"")
            newOrder->SetISIN(headValSplit[1]);
        else if (headValSplit[0] == "\"currency_\"")
            newOrder->SetCurrency(headValSplit[1]);
    }
    orderCollection.insert(std::make_pair(newOrder->GetSecurityId(), std::move(newOrder)));
}

std::vector<std::string> OrderFileHandler::SplitString(const std::string& originalString, char delimiter)
{
    std::vector<std::string> splitStrings{};
    std::istringstream ss(originalString);
    std::string splitString;

    while(std::getline(ss, splitString, delimiter))
        splitStrings.push_back(splitString);

    return splitStrings;
}

void OrderFileHandler::WriteOutputFile(orderCollect& orderCollection)
{
    std::ofstream ordReport(outputFile);

    ordReport << "ISIN\tCurrency\tTotal Buy Count\tTotal Sell Count\tTotal Buy Quantity\tTotal Sell Quantity\t"
              << "Weighted Average Buy Price\tWeighted Average Sell Price\tMax Buy Price\tMin Sell Price" << "\n";

    for(auto& ord : orderCollection)
        ordReport << ord.second->PrintReport() << "\n";

    ordReport.close();
}