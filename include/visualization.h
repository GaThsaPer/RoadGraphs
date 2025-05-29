#pragma once
#include "../include/Node.h"
#include "../include/DownloadData.h"
#include "../include/Vec.h"
#include "../include/Input.h"
#include "../include/Roads.h"

#include <cmath>

#include "raylib.h"
#include "raymath.h"

class window{
    public:
        window() = default;
        void Init();
        void Run();
        void ShutDown();
    private:
        node::Node graph;
        std::vector<int> roads;
        std::vector<int> AdditionalRoads;
        int updateIndexCitiNode;

        Vector2 windowSize;
        bool m_Running;
        RayEngine::Input input;
        Camera2D RenderCamera;

        bool MSTFlag;
        bool AddRoadFlag;
        bool DPDFlag;
        
        void Update(double DeltaTime);
        void Render() const;
        void RenderUI();
};