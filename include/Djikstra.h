#pragma once

#include <queue>
#include <functional>

#include "../include/Node.h"
#include "../include/Vec.h"

namespace path{
    class Djikstra{
        public:
            Djikstra() {}
            Djikstra(int x, node::Node *node, std::vector<int> rd);
            ~Djikstra() {}
            void FindRouteMV(std::vector<int> newRoads);
            void FindRoute(bool Flag);
            void ShowRoute();
            void SetCity(int id) {City = id;}
            void SetNewRoads(std::vector<int> newOne);
            std::vector<int> GetRoute() {return routeTable;}
            std::vector<int> GetJumps() {return jumpsNum;}
            std::vector<std::vector<int>> GetRouteAll() {return routes;}
            std::vector<std::vector<std::vector<int>>> constructAdj(std::vector<int> connBase);
        private:
            int City;
            std::vector<int> MST;
            node::Node *graph;
            std::vector<int> routeTable;
            std::vector<int> jumpsNum;
            std::vector<std::vector<int>> routes;

            //methods
            bool CompareVec(std::vector<int> vec1, std::vector<int> vec2);
    };

};