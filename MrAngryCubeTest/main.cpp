#include "raylib.h"
#include "raymath.h"
#include <string>



int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 800;

int RECT_SIZE = 75;
int HALF_SIZE = RECT_SIZE / 2;

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test Mr. AngryCube in 2d");
    SetTargetFPS(60);

    Matrix transform = MatrixIdentity();

    double lastTime = 0;
    double currentTime = 0;
    float angle = 0;
    int direction = 1;
    int nextDirection = direction;
    
    Vector3 currentPos = Vector3({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0});
    
    transform = MatrixMultiply(MatrixTranslate(currentPos.x, currentPos.y, 0), transform);
    float increment;
    float speed = 3.0;
    float pauseTime = 1.0;
    bool inPauseTime = false;

    while (!WindowShouldClose()) { // Detect window close button or ESC key
        
        currentTime = GetTime();
        if (currentTime - lastTime >= pauseTime) {  // Every second
            lastTime = currentTime;
            inPauseTime = false;
        }

        if (!inPauseTime) {

            increment = PI / 100.0 + sin(angle) / 350.0;
            angle += increment * speed;
            if (angle < PI/2)
            {
                transform = MatrixMultiply(MatrixTranslate(direction*HALF_SIZE, HALF_SIZE, 0), transform);
                transform = MatrixMultiply(MatrixRotateZ(direction*increment*speed), transform);
                transform = MatrixMultiply(MatrixTranslate(direction*-HALF_SIZE, -HALF_SIZE, 0), transform);
            } 
            else
            {
                angle = 0;
                currentPos.x += RECT_SIZE * direction;
                transform = MatrixMultiply(MatrixTranslate(currentPos.x, currentPos.y, currentPos.z), MatrixScale(1, 1, 1));
                
                inPauseTime = true;
                if (currentPos.x >= SCREEN_WIDTH && direction > 0)
                {
                    currentPos.x = -2*RECT_SIZE;
                } else if (currentPos.x <= 0 && direction < 0)
                {
                    currentPos.x = SCREEN_WIDTH + RECT_SIZE;
                }
            }
        } else
        {
            direction = nextDirection;
        }
        
        if (IsKeyPressed(KEY_A))
        {
            nextDirection = -1;
        } else if (IsKeyPressed(KEY_D))
        {
            nextDirection = 1;
        } else if (IsKeyPressed(KEY_W))
        {
            speed += .1;
            pauseTime -= .1;
        } else if (IsKeyPressed(KEY_S))
        {
            speed -= .1;
            pauseTime += .1;
        }

        BeginDrawing();
        
        ClearBackground(Color(0, 60, 120, 255));
        
        Vector2 p1 = Vector2Transform({-HALF_SIZE, -HALF_SIZE}, transform);
        Vector2 p2 = Vector2Transform({-HALF_SIZE, HALF_SIZE}, transform);
        Vector2 p3 = Vector2Transform({HALF_SIZE, -HALF_SIZE}, transform);
        Vector2 p4 = Vector2Transform({HALF_SIZE,  HALF_SIZE}, transform);
        
        
        DrawLine(0, SCREEN_HEIGHT / 2 + HALF_SIZE, 800, SCREEN_HEIGHT / 2 + HALF_SIZE, GRAY);
        
        DrawLine(p1.x, p1.y, p2.x, p2.y, GRAY);
        DrawLine(p3.x, p3.y, p4.x, p4.y, GRAY);
        DrawLine(p2.x, p2.y, p4.x, p4.y, GRAY);
        DrawLine(p1.x, p1.y, p3.x, p3.y, GRAY);
        
        int fps = GetFPS();

        std::string fpsText = TextFormat("FPS: %i\nSPEED: %f", fps, speed);
        DrawText(fpsText.c_str(), 10, 10, 20, GRAY);
        EndDrawing();
    }

    // Cleanup and close the window
    CloseWindow();

    return 0;
}
