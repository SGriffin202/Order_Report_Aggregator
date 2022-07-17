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


/** @brief Calculates the Weighted Average Buy Price
 * 
 *  Will first check that the Buy Quantity is not zero, so that we don't divide by it.
 *  Will return the Total Spent on Buying, which should be 0, if Buy Quantity is 0.
 * 
 *  @return Weighted Average Buy Price
 */
size_t OrderReport::CalcWeightedAvgBuyPrice() const
{
    return ((buyQantity != 0) ? (totalBuySpent/buyQantity) : totalBuySpent);
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
    return ((sellQantity != 0) ? (totalSellSpent/sellQantity) : totalSellSpent);
}


/** @brief Outputs the data needed for the Order Report
 * 
 *  The output report will contain the following headers in the following order:
 *    ISIN | Currency | Total Buy Count | Total Sell Count | Total Buy Quantity | Total Sell Quantity |
 *    Weighted Average Buy Price | Weighted Average Sell Price | Max Buy Price | Min Sell Price
 *
 *  @param delim        - The delimiter that will seperate each value 
 *  @param rptEmptyOrds - Whether to output the security when it has no orders
 *  @return The data needed to write an Order Report
 */
std::string OrderReport::OutputReport(const char delim, const bool rptEmptyOrds) const
{
    std::string output = "";
    if(rptEmptyOrds || buyCount > 0 || sellCount > 0)
    {
        // Longest strings produced are in the high 70's.
        // Reserving the size of 100 should reduce the number of memory allocations needed.
        //
        output.reserve(100);
        output += ISIN;
        output += delim;
        output += currency;
        output += delim;
        output += std::to_string(buyCount);
        output += delim;
        output += std::to_string(sellCount);
        output += delim;
        output += std::to_string(buyQantity);
        output += delim;
        output += std::to_string(sellQantity);
        output += delim;
        output += std::to_string(CalcWeightedAvgBuyPrice());
        output += delim;
        output += std::to_string(CalcWeightedAvgSellPrice());
        output += delim;
        output += std::to_string(maxBuyPrice);
        output += delim;
        output += std::to_string(minSellPrice);
        output += "\n";
    }

    return output;
}