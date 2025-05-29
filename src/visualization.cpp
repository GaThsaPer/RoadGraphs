#include "../include/visualization.h"

void window::Init(){
    Vector2 screenHalfSize = {windowSize.x * 0.5f, windowSize.y * 0.5f};
    windowSize.x = 1280;
    windowSize.y = 720;
    InitWindow(windowSize.x, windowSize.y, "Visualization");
    m_Running = true;
    RenderCamera = {
        .offset = screenHalfSize,
        .target = {0.0f, 0.0f},
        .rotation = 0.0f, 
        .zoom = 1.0f
    };

    MSTFlag = true;
    AddRoadFlag = true;
    DPDFlag = true;
    GPSFlag = true;

    DD::Data Dwn(graph);
    Kruskal kr(graph);
    kr.CreateTree();
    roads = kr.GetRoads();
    AddRoads addR(&graph, 5, roads);
    addR.SetBasicCity("Warszawa");
    addR.CreateRoutes();
    AdditionalRoads = addR.GetNewRoads();

    // graph.ShowAllCities();

    path::Djikstra dji(62, &graph, roads);
    dji.FindRoute(true);
    std::vector<int> Avarage = dji.GetRoute();
    std::vector<int> JumpsNum = dji.GetJumps();
    float Avg = 0, Jp = 0;
    for(int i=0; i < Avarage.size(); i++){
        Avg += Avarage.at(i);
        Jp += JumpsNum.at(i);
    }
    Jp /= (float)(JumpsNum.size() - 1);
    Avg /= (float)(Avarage.size() - 1);
    float Avg_1 = Avg;
    float Jp_1 = Jp;

    std::vector<int> tempVec = roads;
    tempVec.insert(tempVec.end(), AdditionalRoads.begin(), AdditionalRoads.end());
    dji.SetNewRoads(tempVec);
    dji.FindRoute(true);
    Avarage.clear();
    JumpsNum.clear();
    Avarage = dji.GetRoute();
    JumpsNum = dji.GetJumps();
    Avg = 0;
    Jp = 0;
    for(int i=0; i<Avarage.size(); i++){
        Avg += Avarage.at(i);
        Jp += JumpsNum.at(i);
    }
    Jp /= (float)(JumpsNum.size() - 1);
    Avg /= (float)(Avarage.size() - 1);
    printf("\nBefore: [%.4f]km, %.2f jumps\nAfter: [%.4f]km, %.2f jumps\n", Avg_1, Jp_1, Avg, Jp);
    // std::cout << "\n\n" << tempVec.size() << '\n';
    CreateConnections CC(&graph, 20);
    NewConn = CC.GetNewRandConn();
    tempVec.insert(tempVec.end(), NewConn.begin(), NewConn.end());
    CC.SetNewEx(tempVec);
    CC.Create();
    GPSConnection = CC.GetVecConn();

    int sumGPS = 0;
    for(int i=0; i<GPSConnection.size(); i++){
        auto* temp = static_cast<node::ConnectionNode*>(&graph.GetValue(GPSConnection.at(i), node::TypeNode::ROAD));
        sumGPS += temp->GetCost();
    }
    printf("GPS sum: %dkm\n\n\n", sumGPS);

    // kr.ShowTree();
    // graph.ShowAllCities();
    // graph.ShowCostSortConnections();
    // graph.ShowAllConnections();

    updateIndexCitiNode = -1;

    Vector2 coord = {75.0f, 100.0f};
    for(int i=0; i<graph.GetCitySize(); i++){
        auto* temp = static_cast<node::CityNode*>(&graph.GetValue(i, node::TypeNode::CITY));
        temp->SetCoord(coord.x, coord.y);

        if((i+1)%10==0){
            coord.x = 75.0f;
            coord.y += 85.0f;
        }
        else{
            coord.x += 125.0f;
        }
    }

}

void window::Run(){
    double deltaTime = 1.0f / 30.0f;
    while(m_Running && !WindowShouldClose()){
        const double frameStartTime = GetTime();
        input.Handle(windowSize);
        Update(deltaTime);
        BeginDrawing();
        ClearBackground(WHITE);
        const Vector2 ScreenHalfSize = {windowSize.x * 0.5f, windowSize.y * 0.5f};
        RenderCamera.offset = ScreenHalfSize;
        BeginMode2D(RenderCamera);
        Render();
        EndMode2D();
        RenderUI();
        EndDrawing();
        const double frameEndTime  = GetTime();
        deltaTime = frameEndTime - frameStartTime;
        deltaTime = std::min(deltaTime, 1.0/60.0f);
        
    }
}

void window::ShutDown(){
    CloseWindow();
}

void window::Update(double DeltaTime){

    if(input.GetKey(RayEngine::KeyCode::N1, RayEngine::InputState::Pressed))
        MSTFlag = !MSTFlag;
    if(input.GetKey(RayEngine::KeyCode::N2, RayEngine::InputState::Pressed))
        AddRoadFlag = !AddRoadFlag;
    if(input.GetKey(RayEngine::KeyCode::N3, RayEngine::InputState::Pressed))
        DPDFlag = !DPDFlag;
    if(input.GetKey(RayEngine::KeyCode::N4, RayEngine::InputState::Pressed))
        GPSFlag = !GPSFlag;

    if(input.GetMouseButton(RayEngine::MouseButton::Right, RayEngine::InputState::Pressed)){
        Vector2 pos = input.GetCursorPosition();
        printf("{%f}{%f}\n", pos.x, pos.y);
    }
    Vector2 pos = input.GetCursorPosition();

    if(input.GetMouseButton(RayEngine::MouseButton::Left, RayEngine::InputState::Pressed)){
        for(int i=0; i<graph.GetCitySize(); i++){
            auto* temp = static_cast<node::CityNode*>(&graph.GetValue(i ,node::TypeNode::CITY));
            float posX = (pos.x * windowSize.x) - temp->GetX();
            float posY = (pos.y * windowSize.y) - temp->GetY();
            float distance = sqrt((posX * posX) + (posY * posY));
            // printf("[%f]\n{%f}{%f}\n{%f}{%f},{%f}{%f}\n", 
            //     distance, posX, posY, (pos.x * windowSize.x), (pos.y * windowSize.y), temp->GetX(), temp->GetY());
            if(distance < 20 && posX < 20 && posY < 20 && posX > -20 && posY > -20){
                updateIndexCitiNode = i;
                break;
            }
        }
    }
    if(input.GetMouseButton(RayEngine::MouseButton::Left, RayEngine::InputState::Held) && updateIndexCitiNode != -1){
        auto* temp = static_cast<node::CityNode*>(&graph.GetValue(updateIndexCitiNode, node::TypeNode::CITY));
        temp->SetCoord((pos.x * windowSize.x), (pos.y * windowSize.y));
    }
    if(input.GetMouseButton(RayEngine::MouseButton::Left, RayEngine::InputState::Relased)){
        updateIndexCitiNode = -1;
    }
}

void window::Render() const{
    

}

void window::RenderUI(){
    Vector2 ScreenHalfSize = {windowSize.x * 0.5f, windowSize.y * 0.5f};
    if(MSTFlag){
        for(int i=0; i<roads.size(); i++){
            auto* connection = static_cast<node::ConnectionNode*>(&graph.GetValue(roads.at(i), node::TypeNode::ROAD));
            auto* firstCity = static_cast<node::CityNode*>(&graph.GetValue(connection->FirstCity(), node::TypeNode::CITY));
            auto* secoundCity = static_cast<node::CityNode*>(&graph.GetValue(connection->SecoundCity(), node::TypeNode::CITY));
            Vector2 startPos = {firstCity->GetX(), firstCity->GetY()};
            Vector2 endPos = {secoundCity->GetX(), secoundCity->GetY()};
            DrawLineEx(startPos, endPos, 3.0f, GRAY);
        }
    }
    if(AddRoadFlag){
        for(int i=0; i<AdditionalRoads.size(); i++){
            auto* connection = static_cast<node::ConnectionNode*>(&graph.GetValue(AdditionalRoads.at(i), node::TypeNode::ROAD));
            auto* firstCity = static_cast<node::CityNode*>(&graph.GetValue(connection->FirstCity(), node::TypeNode::CITY));
            auto* secoundCity = static_cast<node::CityNode*>(&graph.GetValue(connection->SecoundCity(), node::TypeNode::CITY));
            Vector2 startPos = {firstCity->GetX(), firstCity->GetY()};
            Vector2 endPos = {secoundCity->GetX(), secoundCity->GetY()};
            DrawLineEx(startPos, endPos, 3.0f, RED);
        }
    }
    if(DPDFlag){
        for(int i=0; i<NewConn.size(); i++){
            auto* connection = static_cast<node::ConnectionNode*>(&graph.GetValue(NewConn.at(i), node::TypeNode::ROAD));
            auto* firstCity = static_cast<node::CityNode*>(&graph.GetValue(connection->FirstCity(), node::TypeNode::CITY));
            auto* secoundCity = static_cast<node::CityNode*>(&graph.GetValue(connection->SecoundCity(), node::TypeNode::CITY));
            Vector2 startPos = {firstCity->GetX(), firstCity->GetY()};
            Vector2 endPos = {secoundCity->GetX(), secoundCity->GetY()};
            DrawLineEx(startPos, endPos, 3.0f, GREEN);
        }
    }
    if(GPSFlag){
        for(int i=0; i<GPSConnection.size(); i++){
            auto* connection = static_cast<node::ConnectionNode*>(&graph.GetValue(GPSConnection.at(i), node::TypeNode::ROAD));
            auto* firstCity = static_cast<node::CityNode*>(&graph.GetValue(connection->FirstCity(), node::TypeNode::CITY));
            auto* secoundCity = static_cast<node::CityNode*>(&graph.GetValue(connection->SecoundCity(), node::TypeNode::CITY));
            Vector2 startPos = {firstCity->GetX(), firstCity->GetY()};
            Vector2 endPos = {secoundCity->GetX(), secoundCity->GetY()};
            DrawLineEx(startPos, endPos, 3.0f, ORANGE);
        }
    }

    for(int i=0; i<graph.GetCitySize(); i++){
        auto* temp = static_cast<node::CityNode*>(&graph.GetValue(i ,node::TypeNode::CITY));
        DrawCircle(temp->GetX(), temp->GetY(), 25.0f, ColorAlpha(GREEN, 1.0f));
        std::string str = std::to_string(i);
        std::string txt = temp->city();
        const Vector2 txtSize = MeasureTextEx(
            GetFontDefault(), txt.c_str(), 15.0f, 2.0f
        );
        DrawText(txt.c_str(), (temp->GetX() - txtSize.x * 0.5f), temp->GetY() - 8.5f, 15.0f, BLACK);
    }
}
