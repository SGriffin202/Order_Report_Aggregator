#include "OrderReport.h"

OrderReport::OrderReport()
{
    ISIN = "";
    currency = "";
    securityId = 0;
    buyCount = 0;
    sellCount = 0;
    buyQantity = 0;
    sellQantity = 0;
    maxBuyPrice = 0;
    minSellPrice = 0;
    totalBuySpent = 0;
    totalSellSpent = 0;
}

OrderReport::~OrderReport()
{
}

int OrderReport::GetSecurityId()
{
    return securityId;
}

void OrderReport::SetISIN(std::string& isin)
{
    ISIN = isin;
}

void OrderReport::SetCurrency(std::string& cur)
{
    currency = cur;
}

void OrderReport::SetSecurityId(int secId)
{
    securityId = secId;
}

void OrderReport::AddOrderData(OrderAddData& ordData)
{
    if (ordData.side == 'B')
    {
        buyCount++;
        buyQantity += ordData.quantity;
        if (ordData.price > maxBuyPrice)
            maxBuyPrice = ordData.price;
        totalBuySpent += (ordData.quantity * ordData.price);
    }
    else
    {
        sellCount++;
        sellQantity += ordData.quantity;
        if (ordData.price < minSellPrice)
            minSellPrice = ordData.price;
        totalSellSpent += (ordData.quantity * ordData.price);
    }
}

size_t OrderReport::CalulateWeightedAvgBuyPrice()
{
    return ((buyQantity != 0) ? (totalBuySpent/buyQantity) : totalBuySpent);
}

size_t OrderReport::CalulateWeightedAvgSellPrice()
{
    return ((sellQantity != 0) ? (totalSellSpent/sellQantity) : totalSellSpent);
}

// ISIN | Currency | Total Buy Count | Total Sell Count | Total Buy Quantity | Total Sell Quantity |
// Weighted Average Buy Price | Weighted Average Sell Price | Max Buy Price | Min Sell Price
//
std::string OrderReport::OutputReport(char delim, bool rptEmptyOrds)
{
    std::string output = "";
    if(rptEmptyOrds || buyCount > 0 || sellCount > 0)
    {
        output =  ISIN + delim
                + currency + delim
                + std::to_string(buyCount) + delim
                + std::to_string(sellCount) + delim
                + std::to_string(buyQantity) + delim
                + std::to_string(sellQantity) + delim
                + std::to_string(CalulateWeightedAvgBuyPrice()) + delim
                + std::to_string(CalulateWeightedAvgSellPrice()) + delim
                + std::to_string(maxBuyPrice) + delim
                + std::to_string(minSellPrice)
                + "\n";
    }

    return output;
}