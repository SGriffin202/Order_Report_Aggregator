#include "Order.h"

Order::Order()
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

Order::~Order()
{
}

int Order::GetSecurityId()
{
    return securityId;
}

void Order::SetISIN(std::string& isin)
{
    ISIN = isin;
}

void Order::SetCurrency(std::string& cur)
{
    currency = cur;
}

void Order::SetSecurityId(int secId)
{
    securityId = secId;
}

void Order::AddOrderData(OrderAddData& ordData)
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

size_t Order::CalulateWeightedAvgBuyPrice()
{
    return ((buyQantity != 0) ? (totalBuySpent/buyQantity) : totalBuySpent);
}

size_t Order::CalulateWeightedAvgSellPrice()
{
    return ((sellQantity != 0) ? (totalSellSpent/sellQantity) : totalSellSpent);
}

// ISIN | Currency | Total Buy Count | Total Sell Count | Total Buy Quantity | Total Sell Quantity |
// Weighted Average Buy Price | Weighted Average Sell Price | Max Buy Price | Min Sell Price
//
std::string Order::PrintReport()
{
    std::string output = ISIN + "\t" + currency + "\t" + std::to_string(buyCount) + "\t" + std::to_string(sellCount)
                        + "\t" + std::to_string(buyQantity) + "\t" + std::to_string(sellQantity) + "\t" + 
                        std::to_string(CalulateWeightedAvgBuyPrice()) + "\t" +
                        std::to_string(CalulateWeightedAvgSellPrice()) + "\t" + std::to_string(maxBuyPrice) + "\t" +
                        std::to_string(minSellPrice);
    return output;
}