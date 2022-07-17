/** @file OrderReport.h
 *  @brief Contains everything needed for an Order Report on a Security
 *
 *  This contains the data and functions needed to produce an Order Report on a Security.
 *
 *  @author Sean Griffin
 *  @bug No known bugs.
 */

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

    size_t CalcWeightedAvgBuyPrice() const;
    size_t CalcWeightedAvgSellPrice() const;

public:
    OrderReport();
    ~OrderReport();

    void SetISIN(const std::string& isin_);
    void SetCurrency(const std::string& cur_);
    void SetSecurityId(const int secId_);
    void AddOrderData(const OrderAddData& ordData);
    
    int GetSecurityId() const;
    std::string OutputReport(const char delim, const bool rptEmptyOrds) const;
};

#endif