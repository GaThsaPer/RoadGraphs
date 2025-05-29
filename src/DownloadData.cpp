#include "../include/DownloadData.h"

DD::Data::Data(){
    throw std::runtime_error("Don't get  vector of strings to Data\n");
}

DD::Data::~Data(){}

DD::Data::Data(node::Node &node){
    std::fstream CityData("../data/Cities.txt");
    if(!CityData.is_open())
        throw std::runtime_error("\nCan't open City data\n");
    int iSize = 69;
    node.setSize(iSize);
    std::string line;
    for(int i=0; i<iSize && std::getline(CityData, line); i++){
        node.setCity(i, line);
    }
    CityData.close();
    std::fstream ConnData("../data/Connections.txt");
    if(!ConnData.is_open())
        throw std::runtime_error("\nCan't open Connections data\n");
    iSize = ((iSize * (iSize-1))/2);
    int tempIndex = 0;
    int xID = 0, yID = 1;
    while(std::getline(ConnData, line)){
        int iLineSize = line.size();
        while(!line.empty()){
            
            int tempSize = 0;
            int temp = 0;
            while(temp < iLineSize && line.at(temp) != ','){
                tempSize++;
                temp++;
            }
            std::string str = line.substr(0, tempSize);
            if(line.at(temp == ','))
                line.erase(0, tempSize+1);
            else
                line.clear();
            node.setConn(tempIndex, std::stoi(str), xID, yID);
            tempIndex++;
            iLineSize -= temp+1;
            yID++;
        }
        xID++;
        yID = xID + 1;
    }
    ConnData.close();
}

// DD::Data::Data(node::Node &node){
//     libxl::Book* book = xlCreateXMLBook();
//     std::vector<std::string> data;
//     if(book->load("../data/odleglosci.xlsx")){
//         libxl::Sheet* sheet = book->getSheet(0);
//         if(sheet){
//             for(int row = sheet->firstRow(); row < sheet->lastRow(); ++row){
//                 libxl::CellType cellType = sheet->cellType(row, sheet->firstCol());
//                 if(cellType == libxl::CELLTYPE_STRING){
//                     data.push_back(sheet->readStr(row, sheet->firstCol()));
//                 }
//             }
//             data.erase(data.begin());
//             node.setSize(data.size());
            
//             for(int i=0; i<node.GetCitySize(); i++)
//                 node.setCity(i, data.at(i));
            
//             int tempIndex = 0;
//             for(int row = 1; row < sheet->lastRow(); ++row){
//                 for(int col = row + 1; col < sheet->lastCol(); ++col){
//                     if(sheet->cellType(row, col) == libxl::CELLTYPE_NUMBER){
//                         int temp = sheet->readNum(row, col);
//                         node.setConn(tempIndex, temp, row-1, col-1);
//                         tempIndex++;
//                     }
//                     if(tempIndex % 200 == 0){
//                         book->release();
//                         book = xlCreateXMLBook();
//                         book->load("../data/odleglosci.xlsx");
//                         sheet = book->getSheet(0);
//                     }
//                 }
//                 std::cout << tempIndex << '\n';
//             }
                
//         }
//         else{
//             throw std::runtime_error("\nFailed to create sheet\n");
//         }
//     }
//     else{
//         throw std::runtime_error("\nFailed to open excel file\n");
//     }

//     book->release();
// }