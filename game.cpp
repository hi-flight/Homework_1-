#include <raylib.h>
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

Texture2D background;

bool zoom = false;
int main() {
    /*
    std::ifstream myfile("settings.txt");
    std::string mystring;
    if ( myfile.is_open() ) { // always check whether the file is open
    myfile >> mystring; // pipe file's content into stream
    std::cout << mystring; // pipe stream's content to standard output
    }   
    */

    InitWindow(screenwidth, screenheight, "Lim, Sta. Cruz, Tadiarca_Homework01");
    SetTargetFPS(60);

    background = LoadTexture("I spy.png");
   
    int cam_type;
    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];

    // camera's focus
    camera_view.target = { screenwidth/2, screenheight/2 };
    //camera's offset
    camera_view.offset = { screenwidth/2, screenheight/2 };
    // camera’s zoom
    camera_view.zoom = 1.0f;

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

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera_view);
        DrawTextureEx(background, Vector2{0, 0}, 0, 1, WHITE);
        DrawRectangleLines(box.x, box.y, box.width, box.height, WHITE);
        //DrawRectangle(screenwidth/2, screenheight/2, 400, 400, GREEN);
        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}

