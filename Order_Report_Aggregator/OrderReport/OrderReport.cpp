/** @file OrderReport.cpp
 *  @brief Contains everything needed for an Order Report on a Security
 *
 *  This contains the data and functions needed to produce an Order Report on a Security.
 *
 *  @author Sean Griffin
 *  @bug No known bugs.
 */

#include "OrderReport.h"

OrderReport::OrderReport()
{
    ISIN = "";
    currency = "";
    securityId = 0;
    buyCount = 0;
    sellCount = 0;
    buyQuantity = 0;
    sellQuantity = 0;
    maxBuyPrice = 0;
    minSellPrice = 0;
    totalBuySpent = 0;
    totalSellSpent = 0;
}

OrderReport::~OrderReport()
{
}


/** @brief Gets the Security ID
 * 
 *  @return Security ID
 */
int OrderReport::GetSecurityId() const
{
    return securityId;
}


/** @brief Sets the ISIN
 * 
 *  @param isin - ISIN
 *  @return void
 */
void OrderReport::SetISIN(const std::string& isin_)
{
    ISIN = isin_;
}


/** @brief Sets the Currency
 * 
 *  @param cur - Currency
 *  @return void
 */
void OrderReport::SetCurrency(const std::string& cur_)
{
    currency = cur_;
}


/** @brief Sets the Security ID
 * 
 *  @param secId - Security ID
 *  @return void
 */
void OrderReport::SetSecurityId(const int secId_)
{
    securityId = secId_;
}


/** @brief Adds new Order Data
 * 
 *  When Order Add records (msgType_ = 12) are received then this function can
 *  add the relevant contents of the record to the Order Report object.
 *
 *  @param ordData - The relevant data from the Order Add record
 *  @return void
 */
void OrderReport::AddOrderData(const OrderAddData& ordData)
{
    if (ordData.side == Side::Buy) // In assembly this is doing TEST, as we're checking for 0. A Char would do the "slower" CMP.
    {
        buyCount++;
        buyQuantity += ordData.quantity;
        if (ordData.price > maxBuyPrice)
            maxBuyPrice = ordData.price;
        totalBuySpent += (ordData.quantity * ordData.price);
    }
    else
    {
        sellCount++;
        sellQuantity += ordData.quantity;
        if (ordData.price < minSellPrice)
            minSellPrice = ordData.price;
        totalSellSpent += (ordData.quantity * ordData.price);
    }
}


/** @brief Calculates the Weighted Average Buy Price
 * 
 *  Will first check that the Buy Quantity is not zero, so that we don't divide by it.
 *  Will return the Total Spent on Buying, which should be 0, if Buy Quantity is 0.
 * 
 *  @return Weighted Average Buy Price
 */
size_t OrderReport::CalcWeightedAvgBuyPrice() const
{
    return ((buyQuantity != 0) ? (totalBuySpent/buyQuantity) : totalBuySpent);
}


/** @brief Calculates the Weighted Average Sell Price
 * 
 *  Will first check that the Sell Quantity is not zero, so that we don't divide by it.
 *  Will return the Total Spent on Selling, which should be 0, if Sell Quantity is 0.
 * 
 *  @return Weighted Average Sell Price
 */
size_t OrderReport::CalcWeightedAvgSellPrice() const
{
    return ((sellQuantity != 0) ? (totalSellSpent/sellQuantity) : totalSellSpent);
}


/** @brief Outputs the data needed for the Order Report
 * 
 *  The output report will contain the following headers in the following order:
 *    ISIN | Currency | Total Buy Count | Total Sell Count | Total Buy Quantity | Total Sell Quantity |
 *    Weighted Average Buy Price | Weighted Average Sell Price | Max Buy Price | Min Sell Price
 *
 *  @param outStream    - The stream to output the Order Report to
 *  @param delim        - The delimiter that will seperate each value 
 *  @param rptEmptyOrds - Whether to output the security when it has no orders
 *  @return void
 */
void OrderReport::OutputReport(std::ofstream& outStream, const char delim, const bool rptEmptyOrds) const
{
    if(rptEmptyOrds || buyCount > 0 || sellCount > 0)
    {
        outStream << ISIN << delim
                  << currency << delim
                  << buyCount << delim
                  << sellCount << delim
                  << buyQuantity << delim
                  << sellQuantity << delim
                  << CalcWeightedAvgBuyPrice() << delim
                  << CalcWeightedAvgSellPrice() << delim
                  << maxBuyPrice << delim
                  << minSellPrice
                  << "\n";
    }
}