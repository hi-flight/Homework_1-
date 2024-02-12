#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>

float Lerp(float start, float end, float t) {
    return start + t * (end - start);
}

float Clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

float screenwidth = 800;
float screenheight = 600;

Vector2 targetDestination;

float speed;

// Adjusted the box initialization to be at the center of the screen
Rectangle box;

bool zoom = false;
int main() {
    int cam_type;
    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];

    std::string backgroundFilename;
    Texture2D background;
    std::ifstream inputFile("settings.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Failed to open settings.txt" << std::endl;
        return 1;
    }

    inputFile >> backgroundFilename;
    inputFile >> camera_view.target.x >> camera_view.target.y; // Load camera target
    inputFile >> camera_view.offset.x >> camera_view.offset.y; // Load camera offset
    inputFile >> camera_view.zoom; // Load camera zoom
    inputFile >> EDGE_X[0] >> EDGE_X[1];
    inputFile >> EDGE_Y[0] >> EDGE_Y[1];
    inputFile >> speed;
    inputFile >> box.x >> box.y >> box.width >> box.height;
    
    inputFile.close();

    InitWindow(screenwidth, screenheight, "Lim, Sta. Cruz, Tadiarca_Homework01");
    SetTargetFPS(60);

    background = LoadTexture(backgroundFilename.c_str());

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        targetDestination = GetMousePosition();

        Vector2 worldMousePosition;
        worldMousePosition.x = (targetDestination.x - camera_view.offset.x) / camera_view.zoom + camera_view.target.x;
        worldMousePosition.y = (targetDestination.y - camera_view.offset.y) / camera_view.zoom + camera_view.target.y;

        // Archived Cam Movement code
        if (!zoom){
            if (worldMousePosition.x < box.x + box.width / 2) {
            // Move box left
            box.x -= speed * delta_time;
            }
            if (worldMousePosition.x > box.x + box.width / 2) {
                // Move box right
                box.x += speed * delta_time;
            }
            if (worldMousePosition.y < box.y + 150) {
                box.y -= speed * delta_time;
            }
            //  if (worldMousePosition.x < box.x) {
            //      box.x -= speed * delta_time;
            //  } 
            if (worldMousePosition.y > box.y + 150) {
                box.y += speed * delta_time;
            } 
        }
        
        // Smoothly interpolate camera position towards box position
        camera_view.target.x = Lerp(camera_view.target.x, box.x + box.width / 2, 0.1f);
        camera_view.target.y = Lerp(camera_view.target.y, box.y + box.height / 2, 0.1f);

        // Clamp camera's target position within the specified edges
        camera_view.target.x = Clamp(camera_view.target.x, EDGE_X[0], EDGE_X[1]);
        camera_view.target.y = Clamp(camera_view.target.y, EDGE_Y[0], EDGE_Y[1]);

        // Update box position based on camera's target position
        box.x = camera_view.target.x - box.width / 2;
        box.y = camera_view.target.y - box.height / 2;

        // Handle zoom
        if (IsMouseButtonPressed(0)) {
            zoom = true;
            camera_view.zoom = zoom ? 3.0f : 1.0f;
        }

        if (IsMouseButtonPressed(1)){
            zoom = false;
            camera_view.zoom = zoom ? 3.0f : 1.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera_view);
        DrawTextureEx(background, Vector2{0, 0}, 0, 1, WHITE);
        DrawRectangleLines(box.x, box.y, box.width, box.height, WHITE);
        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}

// for (int i = 0; i < 3; i++){ # 4 Points
//  p = P_triangle [3][i] * p.list[i] * pow(1-t, n-i) * pow(t, i)
// # n = number of points, i = order
// 1 * P0 * (1-t)**(3-0) * (t**0)
// 1 * P1 * (1-t)**(3-1) * (t**1)