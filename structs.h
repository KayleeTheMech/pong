#ifndef STRUCTS_H
#define STRUCTS_H
struct Vector
{
    float x;
    float y;
};

struct Rectangle
{
    float x1, x2;
    float y1, y2;
};
struct MassiveObject
{
    struct Vector position;
    struct Vector speed;
    struct Rectangle rectangle;
};

struct InertialObject
{
    struct Vector position;
    struct Vector speed;
};

struct GameElements
{
    struct MassiveObject player_bat;
    struct MassiveObject opponent_bat;
    struct InertialObject ball;
};

#endif
