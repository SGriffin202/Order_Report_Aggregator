#ifndef ORDERREPORTFILEHANDLER_H
#define ORDERREPORTFILEHANDLER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "InputFileHandler.h"
#include "OutputFileHandler.h"
#include "OrderReport.h"

typedef std::unordered_map<int, std::unique_ptr<OrderReport>> OrderReportCollection;

class OrderReportFileHandler : public InputFileHandler, public OutputFileHandler
{
private:
    
    const std::string MSG_TYPE_SECURITY_REF = "msgType_\":8";
    const std::string MSG_TYPE_ORDER_ADD = "msgType_\":12";
    OrderReportCollection ordRptColl;
    bool reportEmptyOrders;

    void ReadInputData(const std::string& inputLine) override;
    void FindAndUpdateOrderReport(const std::string& inputLine);
    void CreateOrderReport(const std::string& inputLine);
    void WriteOutputData(std::ofstream &outStream) override;

public:
    OrderReportFileHandler( const std::string& inputFile_,
                            const std::string& outputFile_,
                            OrderReportCollection& ordRptColl_,
                            bool reportZeroes_ );
    ~OrderReportFileHandler();
    void SetReportEmptyOrders(bool rptZeroes);
};

#endif