# Order_Report_Aggregator
Produces a TSV (Tab Separated Value) Order Aggregate Report with data from an input file.

Creates a hashed keyed map (unordered_map) of Order Report objects. The map is keyed by Security ID.

Creates an Order Report File Handler object which handles reading in the input file and writing to the output file.

It will read the input file one line at a time and search for the strings "msgType_":8" & "msgType_":12".

It creates an Order Report object and inserts it into the map for Message Type 8. For Message Type 12 it will search the map, and if it finds a Security ID that matches then it will update the related Order Report object.

Once the input file has been fully read the main function will then call the WriteOutputFile() function of the Order Report File Handler object. This will loop through every Order Report object in the map, outputting the required data in a TSV format.

A flag can be set to output securities with no orders against them.
