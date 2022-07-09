#ifndef ORDERREPORT_H
#define ORDERREPORT_H

#include <string>

struct OrderAddData
{
    int securityId;
    char side;
    size_t quantity;
    size_t price;
};

class OrderReport
{
private:
    std::string ISIN;
    std::string currency;
    int securityId;
    int buyCount;
    int sellCount;
    size_t buyQantity;
    size_t sellQantity;
    size_t maxBuyPrice;
    size_t minSellPrice;
    size_t totalBuySpent;
    size_t totalSellSpent;

    size_t CalulateWeightedAvgBuyPrice();
    size_t CalulateWeightedAvgSellPrice();

public:
    OrderReport();
    ~OrderReport();

    void SetISIN(std::string& isin);
    void SetCurrency(std::string& cur);
    void SetSecurityId(int secId);
    void AddOrderData(OrderAddData& ordData);
    
    int GetSecurityId();
    std::string OutputReport(char delim, bool reportZeroes);
};

#endif