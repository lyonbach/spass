#include "ball.h"

void Ball::draw()
{
    DrawCircle(m_pos.x, m_pos.y, m_radius, m_color);
}

void Ball::update()
{
    m_pos.x += m_vel.x;
    m_pos.y += m_vel.y;
}
