#include <string>
#include "OrderReportFileHandler.h"

int main()
{
    const std::string INPUT_FILE  = "pretrade_current.txt";
    const std::string OUTPUT_FILE = "Output_Files/order_report.txt";
    const std::string OUTPUT_FILE_EMPTY_ORDERS = "Output_Files/order_report_including_empty_securities.txt";

    OrderReportCollection ordRptColl {};
    OrderReportFileHandler ordRptFH( INPUT_FILE,    // Input File
                                     OUTPUT_FILE,   // Output File
                                     ordRptColl,    // Map of Order Reports
                                     false );       // Only print Securities that have Orders

    // Read the file defined in INPUT_FILE
    //
    ordRptFH.ReadInputFile();

    // Write to the file OUTPUT_FILE
    // This file will only include Securities that have Orders
    //
    ordRptFH.WriteOutputFile();

    // Set the OrderReportFileHandler to generate a report on Securities that have no Orders as well
    //
    ordRptFH.SetReportEmptyOrders(true);

    // Set a different Ouput file as we don't want to override our existing file
    //
    ordRptFH.SetOutputFile(OUTPUT_FILE_EMPTY_ORDERS);

    // Write to OUTPUT_FILE_EMPTY_ORDERS/
    // This will include Securities that have no Orders as well
    //
    ordRptFH.WriteOutputFile();

    return 0;
}

