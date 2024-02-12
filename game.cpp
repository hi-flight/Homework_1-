#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <raymath.h>
#include <vector>

float screenwidth = 800;
float screenheight = 600;

Vector2 targetDestination;

float speed;

// Adjusted the box initialization to be at the center of the screen
Rectangle box;

struct Character{
    std::string name;
    Vector2 position, min, max;
    bool isfound; 
};

bool zoom = false;
bool objectSelected = false;

int main() {
    int cam_type;
    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];

    std::string backgroundFilename;
    Texture2D background;
    std::ifstream inputFile("settings.txt");

    Character characters[5];
    if (!inputFile.is_open()) {
        std::cerr << "Error: Failed to open settings.txt" << std::endl;
        return 1;
    }

    inputFile >> backgroundFilename;
    for(int i=0; i<5 ; i++){
        inputFile>>characters[i].name>>characters[i].position.x>>characters[i].position.y;
        characters[i].min = {characters[i].position.x, characters[i].position.y};
        characters[i].max = {characters[i].position.x+250, characters[i].position.y+250};
        characters[i].isfound = false;
        std::cout<<"Object #"<<i<<": "<<characters[i].name<<" "<<characters[i].position.x<<" "<<characters[i].position.y<<std::endl;
    }
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
            // Zoom in
                zoom = true;
                camera_view.zoom = zoom ? 3.0f : 1.0f;

                // Adjust camera target position based on mouse position
                camera_view.target.x += (worldMousePosition.x - camera_view.target.x) * (1.0f - 1.0f / camera_view.zoom);
                camera_view.target.y += (worldMousePosition.y - camera_view.target.y) * (1.0f - 1.0f / camera_view.zoom);
            }
        }
        if (zoom){{
            if (IsMouseButtonPressed(0)) {
                for (int i = 0; i < 5; i++) {
                    Rectangle characterBoundingBox = { characters[i].min.x, characters[i].min.y,
                                                    characters[i].max.x - characters[i].min.x,
                                                    characters[i].max.y - characters[i].min.y };

                    if (CheckCollisionPointRec(worldMousePosition, characterBoundingBox)) {
                        // If the mouse is inside the bounding box of the character
                        characters[i].isfound = true;
                        objectSelected = true;
                        // Optionally, you may break the loop if you only want to select the first found character
                        // break;
                    }
                }
            }
        }
        if (IsMouseButtonPressed(1)) {
        // Zoom out
            zoom = false;
            camera_view.zoom = zoom ? 3.0f : 1.0f;

            // Adjust camera target position based on mouse position
            camera_view.target.x += (worldMousePosition.x - camera_view.target.x) * (1.0f - 1.0f / camera_view.zoom);
            camera_view.target.y += (worldMousePosition.y - camera_view.target.y) * (1.0f - 1.0f / camera_view.zoom);
        }
    }
        
    bool allCharactersFound = true;
    for (int i = 0; i < 5; i++) {
        if (!characters[i].isfound) {
            allCharactersFound = false;
            break;
        }
    }
        if (allCharactersFound) {
            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("You found all characters! You win!", screenwidth / 4, screenheight / 2, 30, WHITE);

         EndDrawing();
    
    } else {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera_view);
        DrawTextureEx(background, Vector2{0, 0}, 0, 1, WHITE);
        DrawRectangleLines(box.x, box.y, box.width, box.height, WHITE);
        EndMode2D();
        for(int i = 0; i<5 ; i++){
            Color color;
            if(characters[i].isfound){
                color = GREEN;
            }
            else{
                color = RED;
            }
            DrawText(characters[i].name.c_str(), 10, 25*i, 24, color);
        }       
        EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}

