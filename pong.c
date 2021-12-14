#include <stdbool.h>
#include "pong.h"
#include "structs.h"

void progress_time_in_dimension(float *position_dimension, float *speed_dimension);
bool within_dimension(float pos, float mark1, float mark2);
bool vector_within_massive_object(struct Vector vector, struct MassiveObject object);
void collide_with_object(struct InertialObject *ball, struct MassiveObject *bat);

const float FIELD_LIMIT = 1.0f;
const float BAT_SPEED = 0.05f;

bool move_player_bat_up = false;
bool move_player_bat_down = false;

struct GameElements element_holder = {
    .player_bat = {
        .position = {
            .x = -0.9f, .y = 0.0f},
        .rectangle = {
            .x1 = -0.02f,
            .y1 = -0.14f,
            .x2 = 0.02f,
            .y2 = 0.14f,
        }},
    .ball = {
        .position = {.x = 0.0f, .y = -0.05f},
        .speed = {.x = -0.015f, .y = 0.002f},
    },
};

/*
 * Function:  move_up (bool)
 * --------------------
 * Signals that you want to move the player bat upwards or stop moving upwards.
 */
void move_up(bool value)
{
    move_player_bat_up = value;
}

/*
 * Function:  move_down (bool)
 * --------------------
 * Signals that you want to move the player bat downwards or stop moving downwards.
 */
void move_down(bool value)
{
    move_player_bat_down = value;
}

/*
 * Function:  get_game_elements
 * --------------------
 * Retrieves all GameElements with their current state.
 */
struct GameElements get_game_elements(void)
{
    return element_holder;
}

/*
 * Function:  progress_time
 * --------------------
 * Tells the pong model to calculate the states for the next time step.
 */
void progress_time(void)
{
    struct Vector *position = &(element_holder.ball.position);
    struct Vector *speed = &(element_holder.ball.speed);
    if (move_player_bat_up)
    {
        element_holder.player_bat.position.y = element_holder.player_bat.position.y + BAT_SPEED;
    }
    else if (move_player_bat_down)
    {
        element_holder.player_bat.position.y = element_holder.player_bat.position.y - BAT_SPEED;
    }
    collide_with_object(&(element_holder.ball), &(element_holder.player_bat));
    progress_time_in_dimension(&(position->x), &(speed->x));
    progress_time_in_dimension(&(position->y), &(speed->y));
}

void progress_time_in_dimension(float *position_dimension, float *speed_dimension)
{
    if (*position_dimension + *speed_dimension <= FIELD_LIMIT && *position_dimension + *speed_dimension >= -FIELD_LIMIT)
    {
        *position_dimension = *position_dimension + *speed_dimension;
    }
    if (*position_dimension + *speed_dimension >= FIELD_LIMIT || *position_dimension + *speed_dimension <= -FIELD_LIMIT)
    {
        // no it hit the bounds, reflect the ball
        *position_dimension = *position_dimension - *speed_dimension;
        *speed_dimension = -*speed_dimension;
    }
}

bool within_dimension(float pos, float mark1, float mark2)
{
    return (pos >= mark1 && pos <= mark2) || (pos <= mark1 && pos >= mark2);
}

bool vector_within_massive_object(struct Vector vector, struct MassiveObject object)
{
    struct Rectangle rectangle = {
        .x1 = object.position.x + object.rectangle.x1,
        .y1 = object.position.y + object.rectangle.y1,
        .x2 = object.position.x + object.rectangle.x2,
        .y2 = object.position.y + object.rectangle.y2,
    };
    bool within_x = within_dimension(vector.x, rectangle.x1, rectangle.x2);
    bool within_y = within_dimension(vector.y, rectangle.y1, rectangle.y2);
    return within_x && within_y;
}

void collide_with_object(struct InertialObject *ball, struct MassiveObject *bat)
{
    if (vector_within_massive_object((*ball).position, *bat))
    {
        ball->speed.x = -ball->speed.x;
        ball->speed.y = -ball->speed.y;
    };
}
