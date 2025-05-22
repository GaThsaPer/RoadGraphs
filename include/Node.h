#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace node{
    class CityNode{
        public:
            CityNode();
            CityNode(std::string n);
            ~CityNode();
            void SetCoord(float x, float y);
            void SetCity(std::string str) {name = str;}
            std::string city() {return name;}
        private:
            std::string name;
            float Xcoord;
            float Ycoord;
    };

    class ConnectionNode{
        public:
            ConnectionNode();
            ConnectionNode(int f, int s, int c);
            ~ConnectionNode();
            int FirstCity() {return FirstCityID;}
            int SecoundCity() {return SecoundCityID;}
            int GetCost() {return cost;}
            void SetValues(int First_ID, int Secound_ID, int Cost);
        private:
            int FirstCityID;
            int SecoundCityID;
            int cost;
    };

    class Node{
        public:
            Node();
            void setSize(int iSize);
            void setCity(int index, std::string str);
            void setConn(int index, int num, int firstID, int secoundID);
            int GetCitySize() {return city_size;}
            int GetConnectionsCount() {return (city_size * (city_size - 1)) / 2;}
            void ShowAllCities();
            void ShowAllConnections();
            ~Node();
        private:
            int city_size;
            CityNode* cities;
            ConnectionNode* connections;
    };
};