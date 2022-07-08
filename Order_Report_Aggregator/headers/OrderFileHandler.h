#ifndef ORDERFILEHANDLER_H
#define ORDERFILEHANDLER_H

#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Order.h"

typedef std::unordered_map<int, std::unique_ptr<Order>> orderCollect;

class OrderFileHandler
{
private:
    std::string inputFile;
    std::string outputFile;

    void FindAndUpdateOrder(std::string& inputLine, orderCollect& orderCollection);
    void CreateOrder(const std::string& inputLine, orderCollect& orderCollection);
    std::vector<std::string> SplitString(const std::string& originalString, char delimiter);

public:
    OrderFileHandler(const std::string& inputFile_, const std::string& outputFile_);
    ~OrderFileHandler();

    void ReadInputFile(orderCollect& orderCollection);
    void WriteOutputFile(orderCollect& orderCollection);
};



#endif