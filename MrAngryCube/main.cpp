/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera mode
*
*   Example complexity rating: [★☆☆☆] 1/4
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#define GLSL_VERSION 330

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

void ApplyRotation(Matrix *transform, Vector3 translateBefore, Vector3 rotateAxis, float increment, Vector3 translateAfter) {
    *transform = MatrixMultiply(MatrixTranslate(translateBefore.x, translateBefore.y, translateBefore.z), *transform);
    *transform = MatrixMultiply(MatrixRotate(rotateAxis, increment), *transform);
    *transform = MatrixMultiply(MatrixTranslate(translateAfter.x, translateAfter.y, translateAfter.z), *transform);
}

float QuantizeAngle(float angle) {
    return roundf(angle / (M_PI / 2)) * (M_PI / 2); // Round to nearest 90°
}


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float cubeSize = 2.0f;
    const char* texturePath = "/media/lyonbach/depot/Work/Projects/codeoflyonbach/MrAngryCubeTest/textures/concrete.png";
    // const char* shaderPath = "/media/lyonbach/depot/Work/Projects/codeoflyonbach/vendor/raylib/examples/shaders/resources/shaders/glsl330/base.fs";
    const char* shaderPath = "/media/lyonbach/depot/Work/Projects/codeoflyonbach/vendor/raylib/examples/shaders/resources/shaders/glsl330/blur.fs";
    InitWindow(screenWidth, screenHeight, "Mr Angry Cube Test - V 0.1");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, -5.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    float targetAngle = PI / 2;  // At 90 degrees we change the rotation point
    float increment = roundf((PI / 250.0f) * 100) / 100; // Angle increment with 2 decimal places
    // float angle = 0.0f;
    Vector3 rotateCount = {0, 0, 0};
    Vector3 angleVector = {0, 0, 0};

    Vector3 cubePosition = { 0.0f, cubeSize/2, 0.0f };
    Vector3 rotateAxis = { 0.0f, 0.0f, 1.0f };
    Vector3 nextRotateAxis = rotateAxis;

    Model model = LoadModel("/media/lyonbach/depot/Work/Projects/codeoflyonbach/MrAngryCubeTest/models/mr_angry_cube.obj");
    Texture texture = LoadTexture(texturePath);
    Shader shader = LoadShader(0, shaderPath);
    Material material = LoadMaterialDefault();
    
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_DIFFUSE, texture);
    
    Mesh mrAngryCubeMesh = model.meshes[0];

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Define Variables
    Matrix transform = MatrixIdentity();
    transform = MatrixMultiply(MatrixTranslate(cubePosition.x, cubePosition.y, cubePosition.z), transform);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Handle keypress
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_W))
        {
            nextRotateAxis = { 1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_S))
        {
            nextRotateAxis = { -1.0f, 0.0f, 0.0f };
        } else if (IsKeyPressed(KEY_A))
        {
            nextRotateAxis = { 0.0f, 0.0f, -1.0f };
        } else if (IsKeyPressed(KEY_D))
        {
            nextRotateAxis = { 0.0f, 0.0f, 1.0f };
        }
        //----------------------------------------------------------------------------------
        
        // Update
        //----------------------------------------------------------------------------------
        angleVector = Vector3Add(angleVector, Vector3Scale(rotateAxis, increment));
        if (rotateAxis.x == 1 || rotateAxis.x == -1) {
            angleVector.x = fmod(angleVector.x, 2 * M_PI);
            // angleVector.x = QuantizeAngle(angleVector.x);
            
            float sin_angle = sin(angleVector.x);
            float cos_angle = cos(angleVector.x);
            if (abs(sin_angle) < .005 || abs(cos_angle) < .005) {
                // Apply quantization on every 90° rotation and set the rotate axis to the next axis.
                rotateCount = Vector3Add(rotateCount, rotateAxis);
                angleVector.x = QuantizeAngle(angleVector.x);
                rotateAxis = nextRotateAxis;
            }
            
            if (sin_angle > 0.0f && cos_angle > 0.0f) { // First quarter
                ApplyRotation(&transform, (Vector3){0, -cubeSize/2, cubeSize/2}, rotateAxis, increment, (Vector3){0, cubeSize/2, -cubeSize/2});
            } else if (sin_angle > 0.0f && cos_angle < 0.0f) { // Second quarter
                ApplyRotation(&transform, (Vector3){0, cubeSize/2, cubeSize/2}, rotateAxis, increment, (Vector3){0, -cubeSize/2, -cubeSize/2});
            } else if (sin_angle < 0.0f && cos_angle < 0.0f) { // Third quarter
                ApplyRotation(&transform, (Vector3){0, cubeSize/2, -cubeSize/2}, rotateAxis, increment, (Vector3){0, -cubeSize/2, cubeSize/2});
            } else { // Fourth quarter
                ApplyRotation(&transform, (Vector3){0, -cubeSize/2, -cubeSize/2}, rotateAxis, increment, (Vector3){0, cubeSize/2, cubeSize/2});
            }
        } else if (rotateAxis.z == 1 || rotateAxis.z == -1) {
            angleVector.z = fmod(angleVector.z, 2 * M_PI);
            // angleVector.z = QuantizeAngle(angleVector.z);
        
            float sin_angle = sin(angleVector.z);
            float cos_angle = cos(angleVector.z);
        
            if (abs(sin_angle) < .005 || abs(cos_angle) < .005) {
                // Apply quantization on every 90° rotation and set the rotate axis to the next axis.
                rotateCount = Vector3Add(rotateCount, rotateAxis);
                angleVector.z = QuantizeAngle(angleVector.z);
                rotateAxis = nextRotateAxis;
            }

            if (sin_angle > 0.0f && cos_angle > 0.0f) { // First quarter
                ApplyRotation(&transform, (Vector3){-cubeSize/2, -cubeSize/2, 0}, rotateAxis, increment, (Vector3){cubeSize/2, cubeSize/2, 0});
            } else if (sin_angle > 0.0f && cos_angle < 0.0f) { // Second quarter
                ApplyRotation(&transform, (Vector3){-cubeSize/2, cubeSize/2, 0}, rotateAxis, increment, (Vector3){cubeSize/2, -cubeSize/2, 0});
            } else if (sin_angle < 0.0f && cos_angle < 0.0f) { // Third quarter
                ApplyRotation(&transform, (Vector3){cubeSize/2, cubeSize/2, 0}, rotateAxis, increment, (Vector3){-cubeSize/2, -cubeSize/2, 0});
            } else { // Fourth quarter
                ApplyRotation(&transform, (Vector3){cubeSize/2, -cubeSize/2, 0}, rotateAxis, increment, (Vector3){-cubeSize/2, cubeSize/2, 0});
            }
        }
        

        // TraceLog(LOG_INFO, "angleVector: %f %f %f", angleVector.x, angleVector.y, angleVector.z);
        camera.target = (Vector3){transform.m12, transform.m13, transform.m14};
        camera.position = (Vector3){camera.target.x, camera.target.y + 5, camera.target.z - 10.0f};
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(DARKBLUE);
            BeginMode3D(camera);
                DrawMesh(mrAngryCubeMesh, material, transform);
                DrawGrid(10, 1.0f);
            EndMode3D();
            DrawFPS(10, 10);
            DrawText("Test Mr. Angry Cube", 10, 30, 20, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader);       // Unload shader
    // UnloadTexture(texture);     // Unload texture
    UnloadModel(model);         // Unload model
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
