#pragma once

struct Vec2
{
  public:
    float x;
    float y;

    Vec2() : x(0.0f), y(0.0f)
    {
    }

    Vec2(int _x, int _y) : x((float)_x), y((float)_y)
    {
    }

    Vec2(POINT _pt) : x((float)_pt.x), y((float)_pt.y)
    {
    }
};