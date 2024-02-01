#include <raylib.h>

float screenwidth = 800;
float screenheight = 600;
Vector2 targetDestination;

float speed = 300;

Rectangle box = Rectangle{screenwidth/2 - 150, screenheight/2 - 150, 300, 300};

int main() {
    InitWindow(screenwidth, screenheight, "Lim, Sta. Cruz, Tadiarca_Homework01");
    SetTargetFPS(60);

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

        if (targetDestination.y < box.y) {
            box.y -= targetDestination.y * delta_time;
            targetDestination.y = box.y - 1;
        }
        if (targetDestination.x < box.x) {
            box.x -= speed * delta_time;
            targetDestination.x = box.x - 1;
        } 
        if (targetDestination.y > box.y + 300) {
            box.y += speed * delta_time;
            targetDestination.y = box.y + 1;
        } 
        if (targetDestination.x > box.x + 300) {
            box.x += speed * delta_time;
            targetDestination.x = box.x + 1;
        } 

        camera_view.target = Vector2{ box.x + box.width / 2, box.y + box.height / 2 };

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera_view);
        DrawRectangle(screenwidth/2, screenheight/2, 400, 400, GREEN);
        DrawRectangleLines(box.x, box.y, box.width, box.height, WHITE);
        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
