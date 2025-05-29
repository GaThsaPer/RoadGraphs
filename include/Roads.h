#pragma once

#include "../include/Node.h"
#include "../include/Vec.h"
#include "../include/Djikstra.h"

class Kruskal{
    public:
        Kruskal(node::Node node);
        void CreateTree();
        void ShowTree();
        std::vector<int> GetRoads(){return roads;}
        ~Kruskal();
    private:
        node::Node graph;
        std::vector<VEC::vector2> connections;
        std::vector<int> roads;
        std::vector<std::vector<int>> collection;
        //Methods
        void SortVec();
        bool CheckIntegration(node::ConnectionNode* node);
        int findInCollection(int val);
        void SortRoads();
};

class AddRoads{
    public:
        AddRoads() = default;
        AddRoads(node::Node* node, int ANR, std::vector<int> ex);
        ~AddRoads() {}
        void SetBasicCity(std::string str);
        void SetNewAmount(int num);
        void CreateRoutes();
        std::vector<int> GetNewRoads() {return NewRoads;}
    private:
        node::Node *graph;
        std::vector<int> NewRoads;
        int AmountNewRoads;
        std::vector<int> ExistingRoads;
        int BasicCity;
};

class CreateConnections{
    public:
        CreateConnections() = default;
        CreateConnections(node::Node *gr, int amount, std::vector<int> ex);
        ~CreateConnections() {}
        void Create();
        std::vector<int> GetVecConn() {return newConnections;}
    private:
        node::Node* graph;
        int Amount;
        std::vector<int> Existing;
        std::vector<int> WCities;
        std::vector<int> newConnections;
};