#include <iostream>
#include "../include/DownloadData.h"

void PrintVec(const std::vector<std::string> data);

int main(){
    node::Node graph;
    DD::Data Dwn(graph);

    node::Node roads;

    // graph.ShowAllConnections();
    // graph.ShowAllCities();

    return 0;
}

void PrintVec(const std::vector<std::string> data){
    for(int i=0; i<data.size(); i++){
        printf("%s\n",data.at(i).c_str());
    }
    printf("\n\n%zu\n", data.size());
}