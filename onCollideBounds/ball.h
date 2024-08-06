#pragma once
#include "raylib.h"

class Ball 
{

public:
    Ball(Vector2 pos = Vector2(), float r = 0.0f, Color c = Color(), Vector2 vel = Vector2())
        : m_pos(pos), m_radius(r), m_color(c), m_vel(vel)
        {}

    Vector2 m_pos;
    Vector2 m_vel;
    float m_radius;
    Color m_color;

    void draw();
    void update();

};