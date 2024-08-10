#include <iostream>
#include <vector>
#include <cmath>

#include "raylib.h"
#include "ball.h"


unsigned int WIDTH = 800;
unsigned int HEIGHT = 800;
unsigned int MAXBALLCOUNT = 10000;
float RADIUS = 8.0f;
std::vector<Ball> balls;

bool checkUpdateCollision(Ball& ball)
{
    float dx = WIDTH / 2 - ball.m_pos.x;
    float dy = HEIGHT / 2 - ball.m_pos.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    bool collides = distance >= WIDTH / 2 - ball.m_radius;

    if (collides) {

        // Reflect the ball from the point of collision
        float normalX = dx / distance;
        float normalY = dy / distance;

        // Find the dot product
        float dotProd = ball.m_vel.x * normalX + ball.m_vel.y * normalY;

        // Reflect the ball
        ball.m_vel.x = ball.m_vel.x - 2 * dotProd * normalX;
        ball.m_vel.y = ball.m_vel.y - 2 * dotProd * normalY;
    }

    return collides;

}

void spawnBall()
{
    Color randomColor = Color(
        {
            GetRandomValue(130, 255),
            GetRandomValue(130, 255),
            GetRandomValue(130, 255),
            255
        }
    );

    Vector2 randomPos = Vector2(
        {
            WIDTH / 2 + GetRandomValue(-150, 150),
            HEIGHT / 2 + GetRandomValue(-150, 150)
        }
    );


    Vector2 randomVel = Vector2(
        {
            GetRandomValue(-5, 5),
            GetRandomValue(-5, 5)
        }
    );

    balls.push_back(Ball(randomPos, RADIUS, randomColor, randomVel));

}

void draw(std::vector<Ball>& balls)
{
    for (Ball& b : balls)
    {
        b.draw();
    }
}

void update(std::vector<Ball>& balls)
{
    for (Ball& b : balls)
    {
        b.update();
        if(checkUpdateCollision(b))
        {
            if (balls.size() < MAXBALLCOUNT)
            {
                spawnBall();
            }
        };
        b.draw();
    }
}

void drawRelevantArea()
{
    DrawCircle(WIDTH / 2, HEIGHT / 2, WIDTH / 2, Color({120, 120, 120, 255}));
}

int main(void)
{

    InitWindow(WIDTH, HEIGHT, "onCollideBounds");
    SetTargetFPS(60);

    // Init scene
    balls.push_back(Ball({WIDTH / 2, HEIGHT / 3}, RADIUS, Color(RED), {3, 0}));

    double startTime = GetTime();

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawRelevantArea();

            update(balls);
            // draw(balls);

            DrawFPS(20, 20);

            std::string ballCountText = "Ball Count: " + std::to_string(balls.size());
            DrawText(ballCountText.c_str(), 20, 40, 20, BLACK);

            if (GetTime() - startTime < 5)
            {
                DrawText("Each time a ball hits the bounds, a new ball is spawned!", 120, 200, 20, LIGHTGRAY);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
