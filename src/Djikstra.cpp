#include "../include/Djikstra.h"

path::Djikstra::Djikstra(int x, node::Node *node, std::vector<int> rd){
    if(node == nullptr)
        throw std::invalid_argument("\nNone ptr to graph in Djikstra\n");
    graph = node;
    if(x < 0 || x > graph->GetCitySize())
        throw std::invalid_argument("\nInvalid argument as first city in Djikstra\n");
    City = x;
    if(rd.size() == 0)
        throw std::invalid_argument("MST road in Djikstra is empty");
    MST = rd;
}

void path::Djikstra::FindRouteMV(std::vector<int> newRoads){
    if(graph == nullptr || City < 0 || City > graph->GetCitySize())
        throw std::runtime_error("\nDon't have full data\n");
    
    int V = graph->GetCitySize();
    std::vector<int> dist(V, INT_MAX);
    std::vector<int> Base;
    for(int i=0; i<graph->GetConnectionsCount(); i++){
        Base.push_back(i);
    }
    std::vector<std::vector<std::vector<int>>> adj = constructAdj(Base);
    std::vector<VEC::vector2> mostAttend(newRoads.size(), {0, 0});
    for(int i=0; i<newRoads.size(); i++)
        mostAttend.at(i).x = newRoads.at(i);

    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, std::greater<std::vector<int>>> pq;
    pq.push({City, 0, -1});
    dist.at(City) = 0;
    while(!pq.empty()){
        int cityID = pq.top()[0];
        int prevAttend = pq.top()[2];
        pq.pop();
        for(auto x : adj.at(cityID)){
            int v = x[0];
            int cost = x[1];
            int attend = x[2];
            if((long long)dist.at(v) > (long long)dist.at(cityID) + cost){
                dist.at(v) = dist.at(cityID) + cost;
                pq.push({v, dist.at(v), attend});
                mostAttend.at(attend).y++;
                if(prevAttend != -1)
                    mostAttend.at(prevAttend).y--;
            }
        }
    }
    //newRoads - list of potential new roads
    //mostAttend - {index road, number attended}
    std::sort(mostAttend.begin(), mostAttend.end());
    newRoads.clear();
    for(int i=0; i<mostAttend.size(); i++){
        newRoads.push_back(mostAttend.at(i).x);
    }
    routeTable = newRoads;
}
std::vector<std::vector<std::vector<int>>> path::Djikstra::constructAdj(std::vector<int> connBase){
    std::vector<std::vector<std::vector<int>>> adj(graph->GetCitySize());
    for(int i=0; i<connBase.size(); i++){
        auto* temp = static_cast<node::ConnectionNode*>(&graph->GetValue(connBase.at(i), node::TypeNode::ROAD));
        adj.at(temp->FirstCity()).push_back({temp->SecoundCity(), temp->GetCost()});
        adj.at(temp->SecoundCity()).push_back({temp->FirstCity(), temp->GetCost()});
    }
    return adj;
}
void path::Djikstra::FindRoute(bool Flag){
    if(graph == nullptr || City < 0 || City > graph->GetCitySize())
        throw std::runtime_error("\nDon't have full data\n");
    
    int V = graph->GetCitySize();
    std::vector<int> dist(V, INT_MAX);
    std::vector<int> Base;
    for(int i=0; i<graph->GetConnectionsCount(); i++){
        Base.push_back(i);
    }
    std::vector<std::vector<std::vector<int>>> adj;
    if(Flag)
        adj = constructAdj(MST);
    else
        adj = constructAdj(Base);


    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, std::greater<std::vector<int>>> pq;
    pq.push({City, 0, 0});
    dist.at(City) = 0;

    std::vector<int> jump(graph->GetCitySize(), 0);
    std::vector<std::vector<int>> tempRoutes(graph->GetCitySize());

    while(!pq.empty()){
        int cityID = pq.top()[0];
        int tempJumps = pq.top()[2];
        pq.pop();
        for(auto x : adj.at(cityID)){
            int v = x[0];
            int cost = x[1];
            if((long long)dist.at(v) > (long long)dist.at(cityID) + cost){
                dist.at(v) = dist.at(cityID) + cost;
                jump.at(v) = jump.at(cityID) + 1;
                tempRoutes.at(v) = tempRoutes.at(cityID);
                tempRoutes.at(v).push_back(cityID);
                pq.push({v, dist.at(v), jump.at(v)});
            }
        }
    }
    routeTable = dist;
    jumpsNum = jump;
    routes = tempRoutes;
}

void path::Djikstra::ShowRoute(){
    if(routeTable.size() == 0)
        throw std::runtime_error("\nRoute table is empty\n");
    auto* firstCity = static_cast<node::CityNode*>(&graph->GetValue(City, node::TypeNode::CITY));
    for(int i=0; i<routeTable.size(); i++){
        auto* secoundCity = static_cast<node::CityNode*>(&graph->GetValue(i, node::TypeNode::CITY));
        printf("[%s]->[%s] = %d\n", firstCity->city().c_str(), secoundCity->city().c_str(), routeTable.at(i));
    }
}
bool path::Djikstra::CompareVec(std::vector<int> vec1, std::vector<int> vec2){
    if(vec1.size() != vec2.size())
        return false;
    for(int i=0; i<vec1.size(); i++)
        if(vec1.at(i) != vec2.at(i))
            return false;
    return true;
}

void path::Djikstra::SetNewRoads(std::vector<int> newOne){
    if(newOne.size() == 0)
        throw std::invalid_argument("\nNew vector is empty\n");
    MST = newOne;
}
