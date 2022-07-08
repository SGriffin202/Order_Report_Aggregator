#include <string>
#include "Order.h"
#include "OrderFileHandler.h"

int main()
{
    const std::string INPUT_FILE  = "pretrade_current.txt";
    const std::string OUTPUT_FILE = "order_report.txt";
    
    orderCollect orderCollection {};
    OrderFileHandler orderFH(INPUT_FILE, OUTPUT_FILE);

    orderFH.ReadInputFile(orderCollection);
    orderFH.WriteOutputFile(orderCollection);

    return 0;
}

