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

#include <fstream>
#include <string>

enum class Side { Buy, Sell };

struct OrderAddData
{
    Side side;
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
    size_t buyQuantity;
    size_t sellQuantity;
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
    void OutputReport(std::ofstream& outStream, const char delim, const bool rptEmptyOrds) const;
};

#endif