#ifndef STRUCTS_H
#define STRUCTS_H
struct Vector
{
    float x;
    float y;
};

struct Rectangle
{
    float width;
    float height;
};
struct MassiveObject
{
    struct Vector position;
    struct Vector speed;
    struct Rectangle rectangle;
    float tilt; // tilt angle
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
