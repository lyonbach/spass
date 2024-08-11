#include <iostream>
#include <vector>
#include <raylib.h>

const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 1024;
const unsigned int DEFAULT_SIZE = 50;

const std::string ROCK = "ROCK";
const std::string PAPER = "PAPER";
const std::string SCISSORS = "SCISSORS";

Texture2D rockTexture;
Texture2D paperTexture;
Texture2D scissorsTexture;

class DrawStrategy
{
public:
    virtual void draw(Vector2 position, int size) = 0;
};

class RockDrawStrategy : public DrawStrategy
{
public:
    void draw(Vector2 position, int size) override
    {
        DrawTexture(rockTexture, position.x, position.y, WHITE);
    }
};

class PaperDrawStrategy : public DrawStrategy
{
public:
    void draw(Vector2 position, int size) override
    {
        DrawTexture(paperTexture, position.x, position.y, WHITE);
    }
};

class ScissorDrawStrategy : public DrawStrategy
{
public:
    void draw(Vector2 position, int size) override
    {
        DrawTexture(scissorsTexture, position.x, position.y, WHITE);
    }
};

class MoveStrategy
{
public:
    virtual void move(Vector2 &velocity) = 0;
};

class RockMoveStrategy : public MoveStrategy
{
public:
    void move(Vector2 &velocity) override
    {
        velocity = Vector2({0, 0});
    }
};

class RandomMoveStrategy : public MoveStrategy
{
public:
    void move(Vector2 &velocity) override
    {
        if (GetRandomValue(0, 100) > 97)
        {
            velocity = Vector2(
                {GetRandomValue(-1, 1), GetRandomValue(-1, 1)});
        }
    }
};

class FasterRandomMoveStrategy : public MoveStrategy
{
public:
    void move(Vector2 &velocity) override
    {
        if (GetRandomValue(0, 100) > 97)
        {
            velocity = Vector2(
                {GetRandomValue(-2, 2), GetRandomValue(-2, 2)});
        }
    }
};

class GameObject;

class Game
{
public:
    std::vector<GameObject *> m_GameObjects;
    DrawStrategy *m_RockDrawStrategy;
    DrawStrategy *m_PaperDrawStrategy;
    DrawStrategy *m_ScissorDrawStrategy;
    MoveStrategy *m_RandomMoveStrategy;
    MoveStrategy *m_FasterRandomMoveStrategy;
    MoveStrategy *m_RockMoveStrategy;

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    static Game &getInstance()
    {
        static Game instance;
        return instance;
    }

    std::vector<GameObject *> getGameObjects()
    {
        return m_GameObjects;
    }

    void setGameObjects(std::vector<GameObject *> gameObjects)
    {
        m_GameObjects = gameObjects;
    }

    ~Game()
    {
        delete m_RockDrawStrategy;
        delete m_PaperDrawStrategy;
        delete m_ScissorDrawStrategy;
        delete m_RandomMoveStrategy;
        delete m_FasterRandomMoveStrategy;
        delete m_RockMoveStrategy;
    }

private:
    // Private constructor to prevent instantiation from outside
    Game()
    {
        m_RockDrawStrategy = new RockDrawStrategy();
        m_PaperDrawStrategy = new PaperDrawStrategy();
        m_ScissorDrawStrategy = new ScissorDrawStrategy();
        m_RandomMoveStrategy = new RandomMoveStrategy();
        m_RockMoveStrategy = new RockMoveStrategy();
        m_FasterRandomMoveStrategy = new FasterRandomMoveStrategy();
    }
};

class GameObject
{
protected:
    int m_Size;
    int m_ID;
    std::string m_ObjectType;
    Vector2 m_Position;
    Vector2 m_Velocity;
    DrawStrategy *m_DrawStrategy;
    MoveStrategy *m_MoveStrategy;

public:
    GameObject(Vector2 position, Vector2 velocity, int size, int id)
        : m_Position(position), m_Velocity(velocity), m_Size(size), m_ID(id), m_DrawStrategy(nullptr), m_MoveStrategy(nullptr) {}

    std::string getObjectType()
    {
        return m_ObjectType;
    }
    void update()
    {
        // Check collision with bounds
        if (m_Position.x < 0)
        {
            m_Position.x = 0;
            m_Velocity.x *= -1;
        }
        if (m_Position.x + m_Size > WIDTH)
        {
            m_Position.x = WIDTH - m_Size;
            m_Velocity.x *= -1;
        }
        if (m_Position.y < 0)
        {
            m_Velocity.y *= -1;
            m_Position.y = 0;
        }
        if (m_Position.y + m_Size > HEIGHT)
        {
            m_Velocity.y *= -1;
            m_Position.y = HEIGHT - m_Size;
        }

        m_Position.x += m_Velocity.x;
        m_Position.y += m_Velocity.y;

        m_MoveStrategy->move(m_Velocity);
    }
    void updateObjectType(GameObject *other)
    {
        if (this == other)
            return; // Skip self-collision

        if (CheckCollisionRecs(
                {m_Position.x, m_Position.y, m_Size, m_Size},
                {other->m_Position.x, other->m_Position.y, other->m_Size, other->m_Size}))
        {
            if (m_ObjectType == SCISSORS && other->m_ObjectType == ROCK)
            {
                setObjectType(ROCK);
            }
            else if (m_ObjectType == PAPER && other->m_ObjectType == SCISSORS)
            {
                setObjectType(SCISSORS);
            }
            else if (m_ObjectType == ROCK && other->m_ObjectType == PAPER)
            {
                setObjectType(PAPER);
            }
        }
    }
    void draw()
    {
        m_DrawStrategy->draw(m_Position, m_Size);
    }
    void setVelocity(Vector2 velocity)
    {
        m_Velocity = velocity;
    }
    void setObjectType(std::string objectType)
    {
        Game *game = &Game::getInstance();
        m_ObjectType = objectType;
        if (objectType == ROCK)
        {
            m_DrawStrategy = game->m_RockDrawStrategy;
            m_MoveStrategy = game->m_RockMoveStrategy;
        }
        else if (objectType == PAPER)
        {
            m_DrawStrategy = game->m_PaperDrawStrategy;
            m_MoveStrategy = game->m_RandomMoveStrategy;
        }
        else if (objectType == SCISSORS)
        {
            m_DrawStrategy = game->m_ScissorDrawStrategy;
            m_MoveStrategy = game->m_FasterRandomMoveStrategy;
        }
    }
    void setMoveStrategy(MoveStrategy *moveStrategy)
    {
        m_MoveStrategy = moveStrategy;
    }
    void setDrawStrategy(DrawStrategy *drawStrategy)
    {
        m_DrawStrategy = drawStrategy;
    }
    bool operator==(const GameObject &other)
    {
        return m_ID == other.m_ID;
    }
};

Vector2 getRandomVectorInBounds()
{
    return Vector2(
        {GetRandomValue(100, WIDTH - 100),
         GetRandomValue(100, HEIGHT - 100)});
}

std::string getRandomObjectType()
{
    int randomValue = GetRandomValue(0, 2);
    if (randomValue == 0)
    {
        return ROCK;
    }
    if (randomValue == 1)
    {
        return PAPER;
    }
    if (randomValue == 2)
    {
        return SCISSORS;
    }
    return ROCK; // Default case
}

int main(int argc, char *argv[])
{

    InitWindow(WIDTH, HEIGHT, "Rock Paper Scissor");

    rockTexture = LoadTexture("../textures/rock.png");
    rockTexture.width = DEFAULT_SIZE;
    rockTexture.height = DEFAULT_SIZE;

    paperTexture = LoadTexture("../textures/paper.png");
    paperTexture.width = DEFAULT_SIZE;
    paperTexture.height = DEFAULT_SIZE;

    scissorsTexture = LoadTexture("../textures/scissors.png");
    scissorsTexture.width = DEFAULT_SIZE;
    scissorsTexture.height = DEFAULT_SIZE;

    // Create game instance
    std::vector<GameObject *> gameObjects;
    for (int i = 0; i < 45; i++)
    {
        std::string objectType = getRandomObjectType();
        GameObject *go = new GameObject(getRandomVectorInBounds(), {0, 0}, DEFAULT_SIZE, i);
        if (objectType == ROCK)
        {
            go->setObjectType(objectType);
        }
        else if (objectType == PAPER)
        {
            go->setObjectType(objectType);
        }
        else if (objectType == SCISSORS)
        {
            go->setObjectType(objectType);
        }
        gameObjects.push_back(go);
    }

    Game *game = &Game::getInstance();
    game->setGameObjects(gameObjects);

    SetTargetFPS(60);
    double startTime = GetTime();
    bool paused = false;
    bool isRecording = false;
    int frameCounter = 0;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (!paused)
        {

            for (GameObject *go : game->m_GameObjects)
            {
                for (GameObject *other : game->m_GameObjects)
                {
                    go->updateObjectType(other);
                }

                go->update();
                go->draw();
            }

            // Draw FPS
            // DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, LIGHTGRAY);
        }
        else
        {
            DrawText("PAUSED", 450, 200, 20, LIGHTGRAY);
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            paused = !paused;
        }
        if (IsKeyPressed(KEY_R))
        {
            isRecording = !isRecording;
        }

        if (GetTime() - startTime < 10)
        {
            DrawText("Rock, Paper, Scissors, Shoot!", 350, 200, 20, LIGHTGRAY);
        }

        EndDrawing();
        if (isRecording)
        {
            TakeScreenshot(TextFormat("../screenshots/screenshot_%04i.png", frameCounter++));
        }
    }

    for (GameObject *go : game->m_GameObjects)
    {
        delete go;
    }

    return 0;
}