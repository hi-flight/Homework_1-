#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>

float Lerp(float start, float end, float t){
    return start + t * (end - start);
}

float screenwidth = 800;
float screenheight = 600;

Vector2 targetDestination;

float speed = 300;

Rectangle box = Rectangle{screenwidth/2 - 150, screenheight/2 - 150, 300, 300};

bool zoom = false;
int main() {
    int cam_type;
    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];

    std::string backgroundFilename;
    Texture2D background;
    std::ifstream inputFile("settings.txt");

    if (!inputFile.is_open()){
        std::cerr << "Error: Failed to open settings.txt" << std::endl;
        return 1;
    }
    
    inputFile >> backgroundFilename;
    inputFile >> camera_view.target.x >> camera_view.target.y; // Load camera target
    inputFile >> camera_view.offset.x >> camera_view.offset.y; // Load camera offset
    inputFile >> camera_view.zoom; // Load camera zoom
    inputFile.close();

    InitWindow(screenwidth, screenheight, "Lim, Sta. Cruz, Tadiarca_Homework01");
    SetTargetFPS(60);
   
    background = LoadTexture(backgroundFilename.c_str());

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        targetDestination = GetMousePosition();
    
        // Needs clamping
        if (targetDestination.y < box.y) {
            box.y -= speed * delta_time;
            targetDestination.y = box.y - 1;
        }
        if (targetDestination.x < box.x) {
            box.x -= speed * delta_time;
            targetDestination.x = box.x - 1;
        } 
        if (targetDestination.y > box.y) {
            box.y += speed * delta_time;
            targetDestination.y = box.y + 1;
        } 
        if (targetDestination.x > box.x) {
            box.x += speed * delta_time;
            targetDestination.x = box.x + 1;
        } 

        if(IsMouseButtonPressed(0)){
            zoom = !zoom;
            if (zoom){
                camera_view.zoom = 3.0f;
            } 
            else {
                camera_view.zoom = 1.0f;
            }
        }

        // Smoothly interpolate camera position towards box position
        camera_view.target.x = Lerp(camera_view.target.x, box.x + box.width / 2, 0.1f);
        camera_view.target.y = Lerp(camera_view.target.y, box.y + box.height / 2, 0.1f);

        Vector2 rectanglePosition = {
            camera_view.target.x - camera_view.offset.x - (box.width/2),
            camera_view.target.y - camera_view.offset.y - (box.height/2)
        };

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera_view);
        DrawTextureEx(background, Vector2{0, 0}, 0, 1, WHITE);
        DrawRectangleLines(camera_view.target.x, camera_view.target.y, box.width, box.height, WHITE);
        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
