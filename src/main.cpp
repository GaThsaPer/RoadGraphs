#include <iostream>
#include <../include/visualization.h>


void PrintVec(const std::vector<std::string> data);

int main(){
    window WD;
    WD.Init();
    WD.Run();
    WD.ShutDown();
    return 0;
}

void PrintVec(const std::vector<std::string> data){
    for(int i=0; i<data.size(); i++){
        printf("%s\n",data.at(i).c_str());
    }
    printf("\n\n%zu\n", data.size());
}