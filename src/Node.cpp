#include "../include/Node.h"

//CityNodes
node::CityNode::CityNode(): name("NULL"){}
node::CityNode::CityNode(std::string n): name(n){}
node::CityNode::~CityNode() {}
void node::CityNode::SetCoord(float x, float y){
    Xcoord = x;
    Ycoord = y;
}

//ConnectionNodes
node::ConnectionNode::ConnectionNode() {}
node::ConnectionNode::ConnectionNode(int f, int s, int c) {
    if(f < 0)
        throw std::range_error("\nFirst city ID is too small\n");
    FirstCityID = f;
    if(s < 0)
        throw std::range_error("\nSecound city ID is too small\n");
    SecoundCityID = s;
    if(c < 0)
        throw std::range_error("\nRoad cost is too small\n");
    cost = c;
}
node::ConnectionNode::~ConnectionNode() {}
void node::ConnectionNode::SetValues(int First_ID, int Secound_ID, int Cost){
    FirstCityID = First_ID;
    SecoundCityID = Secound_ID;
    cost = Cost;
}

//Node
node::Node::Node() {
    city_size = 0;
}
node::Node::~Node() {}

void node::Node::setSize(int iSize){
    if(city_size != 0)
        throw std::runtime_error("\nThe size has been set\n");
    if(city_size < 0)
        throw std::invalid_argument("\nSize of cities array is too small\n");
    cities = new CityNode [iSize];
    int temp = (iSize*(iSize-1))/2;
    connections = new ConnectionNode [temp];
    city_size = iSize;
}
void node::Node::setCity(int index, std::string str){
    if(cities[index].city() != "NULL")
        throw std::invalid_argument("\nCity already has been set\n");
    cities[index].SetCity(str);
}
void node::Node::setConn(int index, int num, int firstID, int secoundID){
    if(num < 0){
        printf("\n[%d] ", num);
        throw std::invalid_argument("Cost is too small\n");
    }
    if(index < 0 || index > ((city_size * (city_size - 1))/2) - 1){
        printf("\n[%d] ", index);
        throw std::invalid_argument("Index is out of rage\n");
    }
    if(firstID < 0 || firstID > city_size - 1){
        printf("\n[%d] ", firstID);
        throw std::invalid_argument("Invalid first city ID\n");
    }
    if(secoundID < 0 || secoundID > city_size - 1){
        printf("\n[%d] ", secoundID);
        throw std::invalid_argument("Invalid secound city ID\n");
    }
    connections[index].SetValues(firstID, secoundID, num);
}

node::BaseNode& node::Node::GetValue(int index, TypeNode type){
    switch(type){
        case TypeNode::CITY:
            if(index < 0 || index >= city_size)
                throw std::invalid_argument("\nIncorrect index in GetValue() method\n");
            return cities[index];
        break;
        case TypeNode::ROAD:
            if(index < 0 || index >= GetConnectionsCount())
                throw std::invalid_argument("\nIncorrect index in GetValue() method\n");
            return connections[index];
        break;
    };
}

void node::Node::ShowAllCities(){
    if(cities[0].city() == "NULL")
        std::cerr << "\nAll cities are empty\n";
    else{
        for(int i=0; i<city_size; i++){
            printf("%s [%d]\n", cities[i].city().c_str(), i);
        }
    }
}
void node::Node::ShowAllConnections(){
    int temp = (city_size * (city_size - 1))/2;
    for(int i=0; i<temp; i++){
        printf("[%s] -> [%s] = {%d}\n", 
            cities[connections[i].FirstCity()].city().c_str(), 
            cities[connections[i].SecoundCity()].city().c_str(), 
            connections[i].GetCost()
        );
    }
}
void node::Node::ShowCostSortConnections(){
    std::vector<VEC::vector2> conn;
    for(int i=0; i<GetConnectionsCount(); i++){
        conn.push_back(VEC::vector2(i, connections[i].GetCost()));
    }
    for(int i=1; i<conn.size(); i++){
        for(int j=0; j<i; j++){
            if(conn.at(i).y < conn.at(j).y){
                auto temp = conn.at(i);
                conn.erase(conn.begin() + i);
                conn.insert(conn.begin() + j, temp);
                break;
            }
        }
    }
    for(int i=0; i<conn.size(); i++){
        printf("[%s]->[%s]={%d}\n", 
            cities[connections[conn.at(i).x].FirstCity()].city().c_str(),
            cities[connections[conn.at(i).x].SecoundCity()].city().c_str(),
            conn.at(i).y
        );
    }
}
