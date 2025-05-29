#include "../include/Roads.h"

Kruskal::Kruskal(node::Node node): graph(node) {
    node::CityNode* CheckCorrectGraph = static_cast<node::CityNode*>(&graph.GetValue(0, node::TypeNode::CITY));
    if(CheckCorrectGraph->city() == "NULL")
        throw std::invalid_argument("\nGraph is empty\n");
    for(int i=0; i<graph.GetConnectionsCount(); i++){
        auto* temp = static_cast<node::ConnectionNode*>(&graph.GetValue(i, node::TypeNode::ROAD));
        connections.push_back(VEC::vector2(i, temp->GetCost()));
    }
    // SortVec();
    std::sort(connections.begin(), connections.end());
    for(int i=0; i<graph.GetCitySize(); i++){
        collection.push_back(std::vector<int>());
        collection.at(i).push_back(i);
    }
}
Kruskal::~Kruskal() {}

int Kruskal::findInCollection(int val){
    for(int i=0; i<graph.GetCitySize(); i++){
        for(int j = 0; j < collection.at(i).size(); j++){
            if(collection.at(i).at(j) == val)
                return i;
        }
    }
    throw std::runtime_error("\nCan't find that ID in collection\n");
}

void Kruskal::SortRoads(){
    for(int i=1; i<roads.size(); i++){
        auto* tempComp = static_cast<node::ConnectionNode*>(&graph.GetValue(roads.at(i), node::TypeNode::ROAD));
        for(int j=0; j<i; j++){
            auto* temp = static_cast<node::ConnectionNode*>(&graph.GetValue(roads.at(j), node::TypeNode::ROAD));
            if(temp->FirstCity() > tempComp->FirstCity()){
                int tempInt = roads.at(i);
                roads.erase(roads.begin() + i);
                roads.insert(roads.begin() + j, tempInt);
                break;
            }
        }
    }
}
void Kruskal::SortVec(){
    for(int i=1; i<connections.size(); i++){
        for(int j=0; j<i; j++){
            if(connections.at(i).y < connections.at(j).y){
                auto temp = connections.at(i);
                connections.erase(connections.begin() + i);
                connections.insert(connections.begin() + j, temp);
                break;
            }
        }
    }
}
bool Kruskal::CheckIntegration(node::ConnectionNode* node){
    int firstCity = node->FirstCity();
    int secoundCity = node->SecoundCity();
    int frtCtLoc = findInCollection(firstCity);
    for(int i=0; i<collection.at(frtCtLoc).size(); i++){
        if(collection.at(frtCtLoc).at(i) == secoundCity)
            return false;
    }
    int scCtLoc = findInCollection(secoundCity);
    int tempSize = collection.at(scCtLoc).size();
    for(int i=0; i<tempSize; i++){
        collection.at(frtCtLoc).push_back(collection.at(scCtLoc).at(0));
        collection.at(scCtLoc).erase(collection.at(scCtLoc).begin());
    }
    return true;
}

void Kruskal::CreateTree(){
    
    for(int i = 0; roads.size() < graph.GetCitySize() - 1 && i < graph.GetConnectionsCount(); i++){
        auto* temp = static_cast<node::ConnectionNode*>(&graph.GetValue(connections.at(i).x, node::TypeNode::ROAD));
        if(CheckIntegration(temp)){
            roads.push_back(connections.at(i).x);
        }
    }
}

void Kruskal::ShowTree(){
    int iRoadsLength = 0;
    SortRoads();
    for(int i=0; i<roads.size(); i++){
        auto* temp = static_cast<node::ConnectionNode*>(&graph.GetValue(roads.at(i), node::TypeNode::ROAD));
        auto* tempCity1 = static_cast<node::CityNode*>(&graph.GetValue(temp->FirstCity(), node::TypeNode::CITY));
        auto* tempCity2 = static_cast<node::CityNode*>(&graph.GetValue(temp->SecoundCity(), node::TypeNode::CITY));
        iRoadsLength += temp->GetCost();
        // printf("[%s] -> [%s] = {%d}\n", tempCity1->city().c_str(), tempCity2->city().c_str(), temp->GetCost());
        printf("[%d] -> [%d] = {%d}\n", temp->FirstCity(), temp->SecoundCity(), temp->GetCost());
    }
        
    printf("\n\n[%zu], %d\n", roads.size(), iRoadsLength);
}


// AddNewRoads class
AddRoads::AddRoads(node::Node *node, int ANR, std::vector<int> ex){
    if(node == nullptr)
        throw std::invalid_argument("\nInvalit graph pointer\n");
    graph = node;
    if(ANR < 1)
        throw std::invalid_argument("\nThe amount of new roads is too small\n");
    AmountNewRoads = ANR;
    BasicCity = -1;
    if(ex.size() == 0)
        throw std::invalid_argument("\nExisting roads in AddRoads is empty\n");
}
void AddRoads::SetBasicCity(std::string str){
    bool FindFlag = false;
    if(str.size() == 0)
        throw std::invalid_argument("\nCity string is empty\n");
    for(int i=0; i<graph->GetCitySize(); i++){
        auto* temp = static_cast<node::CityNode*>(&graph->GetValue(i, node::TypeNode::CITY));
        if(temp->city() == str){
            BasicCity = i;
            FindFlag = true;
            break;
        }
    }
    if(!FindFlag)
        throw std::runtime_error("\nCan't find that city in graph\n");
}
void AddRoads::SetNewAmount(int num){
    if(num < 1)
        throw std::invalid_argument("\nToo small amount of new roads\n");
    AmountNewRoads = num;
}

void AddRoads::CreateRoutes(){
    std::vector<int> searchingConn = ExistingRoads;
    std::vector<int> newTrace;
    int iCounter = 0;
    for(int i=0; i<graph->GetConnectionsCount(); i++){
        auto* temp = static_cast<node::ConnectionNode*>(&graph->GetValue(i, node::TypeNode::ROAD));
        if(temp->FirstCity() == BasicCity || temp->SecoundCity() == BasicCity){
            iCounter++;
            bool CopyFlag = false;
            for(int j=0; j<ExistingRoads.size(); j++){
                if(i == ExistingRoads.at(j)){
                    CopyFlag = true;
                    break;
                }
            }
            if(!CopyFlag){
                searchingConn.push_back(i);
                newTrace.push_back(i);
            }
            if(iCounter == graph->GetCitySize())
                break;
        }
    }
    path::Djikstra dji(BasicCity, graph, searchingConn);
    dji.FindRouteMV(newTrace);
    std::vector<int> sortNewRoads = dji.GetRoute();
    // std::cout << "\n\n";
    int tempSize = sortNewRoads.size() - AmountNewRoads;
    sortNewRoads.erase(sortNewRoads.begin() + AmountNewRoads, sortNewRoads.end());
    NewRoads = sortNewRoads;
    // std::cout << "\n\n\n\n";
    // for(int i=0; i<NewRoads.size(); i++){
    //     auto* temp = static_cast<node::ConnectionNode*>(&graph->GetValue(NewRoads.at(i), node::TypeNode::ROAD));
    //     auto* City1 = static_cast<node::CityNode*>(&graph->GetValue(temp->FirstCity(), node::TypeNode::CITY));
    //     auto* City2 = static_cast<node::CityNode*>(&graph->GetValue(temp->SecoundCity(), node::TypeNode::CITY));
    //     printf("[%s]->[%s] = %d\n", City1->city().c_str(), City2->city().c_str(), temp->GetCost());
    // }
}


CreateConnections::CreateConnections(node::Node *gr, int amount){
    if(gr == nullptr)
        throw std::invalid_argument("\nGraph value is nullptr in CreateConnections\n");
    graph = gr;
    if(amount < 0 || amount > graph->GetCitySize())
        throw std::invalid_argument("\nInvalid value as amount new connections in CreateConnections\n");
    Amount = amount;
    WCities.push_back(3);
    WCities.push_back(5);
    WCities.push_back(13);
    WCities.push_back(14);
    WCities.push_back(20);
    WCities.push_back(21);
    WCities.push_back(26);
    WCities.push_back(32);
    WCities.push_back(34);
    WCities.push_back(38);
    WCities.push_back(40);
    WCities.push_back(45);
    WCities.push_back(48);
    WCities.push_back(54);
    WCities.push_back(60);
    WCities.push_back(62);
    WCities.push_back(64);
    WCities.push_back(68);
}
void CreateConnections::SetNewEx(std::vector<int> newEx) {
    Existing.clear();
    Existing = newEx;
    path::Djikstra dij(0, graph, Existing);
    for(int i=0; i < graph->GetCitySize(); i++){
        dij.SetCity(i);
        dij.FindRoute(true);
        cost.push_back(dij.GetRoute());
        WCRoutes.push_back(dij.GetRouteAll());
    }
}
void CreateConnections::Create(){
    std::vector<std::vector<std::vector<int>>> adj = createList();
    for(int i=0; adj.size() > WCities.size(); i++){
        if(adj.at(i).size() == 0){
            adj.erase(adj.begin() + i);
            i--;
        }
    }
    int mstCost = 0;
    
    std::vector<std::vector<int>> MST = findMST(adj, mstCost);
    
    std::vector<bool> visited(graph->GetConnectionsCount(), false);
    std::vector<std::vector<std::vector<int>>> mstAdj(adj.size());
    std::vector<std::vector<int>> convMST;
    for(int i=0; i<MST.size(); i++){
        for(int j=0; j<WCities.size(); j++){
            if(WCities.at(j) == MST.at(i).at(0)){
                for(int k=0; k<WCities.size(); k++){
                    if(WCities.at(k) == MST.at(i).at(1)){
                        convMST.push_back({j, k, MST.at(i).at(2)});
                    }
                }
            }
        }
    }
    for(auto e: convMST){

        mstAdj.at(e.at(0)).push_back({e.at(1), e.at(2)});
        mstAdj.at(e.at(1)).push_back({e.at(0), e.at(2)});
    }

    std::vector<int> tour;/////////////////////////////////////
    
    eulerianCircuit(mstAdj, WCities.at(0), tour, visited, -1);
    
    tour.push_back(WCities.at(0));

    std::vector<std::vector<int>> tourPath;

    for(int i = 0; i < tour.size() - 1; i++) {
        int u = tour[i];
        int v = WCities.at(tour[i + 1]);
        int weight = 0;

        for(auto neighbor: adj[u]) {
            if(neighbor[0] == v) {
                weight = neighbor[1];
                break;
            }
        }

        tourPath.push_back({u, v, weight});
    }
    //WCRoutes
    int potSize = 0;
    for(int i=0; i<tourPath.size(); i++){
        int from = tourPath.at(i).at(0);
        int to = tourPath.at(i).at(1);
    std::vector<VEC::vector2> newTraces;
    for(int i=0; i<tourPath.size(); i++){
        int from = tourPath.at(i).at(0);
        int to = tourPath.at(i).at(1);
        for(int j = 0; j<WCRoutes.at(from).at(to).size() - 1; j++){
            newTraces.push_back({WCRoutes.at(from).at(to).at(j), WCRoutes.at(from).at(to).at(j+1)});
        }
        newTraces.push_back({WCRoutes.at(from).at(to).at(WCRoutes.at(from).at(to).size() - 1), to});
    }
    
    std::vector<int> finalTab;
    for(int i=0; i<newTraces.size(); i++){
        int firstCity = newTraces.at(i).x;
        int secoundCity = newTraces.at(i).y;
        for(int j=0; j<graph->GetConnectionsCount(); j++){
            auto* temp = static_cast<node::ConnectionNode*>(&graph->GetValue(j, node::TypeNode::ROAD));
            if((temp->FirstCity() == firstCity && temp->SecoundCity() == secoundCity) || (temp->FirstCity() == secoundCity && temp->SecoundCity() == firstCity)){
                finalTab.push_back(j);
                break;
            }
        }
    }
    std::vector<int> copyElements;
    for(int i=0; i<finalTab.size() - 1; i++){
        for(int j=i+1; j<finalTab.size(); j++){
            if(finalTab.at(i) == finalTab.at(j)){
                finalTab.erase(finalTab.begin() + j);
            }
        }
    }

    newConnections = finalTab;
    
}
}
std::vector<int> CreateConnections::GetNewRandConn(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, graph->GetConnectionsCount());
    std::vector<int> roads;
    for(int i=0; i < Amount; i++){
        roads.push_back(distr(gen));
        for(int j=0; j<Existing.size(); j++){
            if(roads.at(i) == Existing.at(j)){
                roads.erase(roads.end());
                i--;
            }
        }
    }
    return roads;
}
std::vector<std::vector<std::vector<int>>> CreateConnections::createList() {
    int n = cost.size();
    std::vector<std::vector<std::vector<int>>> adj(n);
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (u != v) {
                if(inVec(u) && inVec(v)){
                    adj[u].push_back({v, cost[u][v]});
                }
            }
        }
    }
    return adj;
}

std::vector<std::vector<int>> CreateConnections::findMST(std::vector<std::vector<std::vector<int>>> &adj, int &mstCost) {
    int n = adj.size();

    // to marks the visited nodes
    std::vector<bool> visited(n, false);

    // stores edges of minimum spanning tree
    std::vector<std::vector<int>> mstEdges ;

    std::priority_queue<std::vector<int>, 
        std::vector<std::vector<int>>, std::greater<std::vector<int>>> pq;

    pq.push({0, WCities.at(0), -1});

    while(!pq.empty()) {
        std::vector<int> current = pq.top();
        pq.pop();

        int u = current[1];
        int weight = current[0];
        int parent = current[2];

        if(visited[u]) continue;

        mstCost += weight;
        visited[u] = true;

        if(parent != -1) {
            mstEdges.push_back({u, parent, weight});
        }

        for(auto neighbor: adj[u]) {
            int v = neighbor[0];
            if(v == parent) continue;
            int w = neighbor[1];

            if(!visited[v]) {
                pq.push({w, v, u});
            }
        }
    }
    return mstEdges;
}
void CreateConnections::eulerianCircuit(std::vector<std::vector<std::vector<int>>> &adj, int u, std::vector<int> &tour, std::vector<bool> &visited, int parent) {
    visited[u] = true;
    tour.push_back(u);

    for(auto neighbor: adj[u]) {
        int v = neighbor[0];
        if(v == parent) continue;

        if(!visited[v]) {
            eulerianCircuit(adj, v, tour, visited, u);
        }
    }
}

bool CreateConnections::inVec(int val){
    for(int i=0; i<WCities.size(); i++)
        if(val == WCities.at(i))
            return true;
    return false;
}