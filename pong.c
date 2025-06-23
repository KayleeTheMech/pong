#include <stdbool.h>

#define _POSIX_C_SOURCE 199309L
#include "pong.h"
#include "structs.h"
#include <time.h>

// declarations for this file
void progress_time_in_dimension(float *position_dimension, float *speed_dimension);
bool within_dimension(float pos, float mark1, float mark2);
bool vector_within_massive_object(struct Vector vector, struct MassiveObject object);
void collide_with_object(struct InertialObject *ball, struct MassiveObject *bat);
void move_bats(void);
void ai_routine(void);

const float FIELD_LIMIT = 1.0f;
const float BAT_SPEED = 0.02f;

struct GameElements element_holder = {
    .player_bat = {.position = {.x = -0.9f, .y = 0.0f},
                   .rectangle =
                       {
                           .x1 = -0.02f,
                           .y1 = -0.14f,
                           .x2 = 0.02f,
                           .y2 = 0.14f,
                       },
                   .speed = {.x = 0.0f, .y = 0.0f}},
    .opponent_bat =
        {
            .position = {.x = 0.9f, .y = 0.0f},
            .rectangle =
                {
                    .x1 = -0.02f,
                    .y1 = -0.14f,
                    .x2 = 0.02f,
                    .y2 = 0.14f,
                },
            .speed = {.x = 0.0f, .y = 0.0f},
        },
    .ball =
        {
            .position = {.x = 0.0f, .y = -0.05f},
            .speed = {.x = +0.02f, .y = 0.008f},
        },
};

/*
 * Function:  move_up (bool)
 * --------------------
 * Signals that you want to move the player bat upwards or stop moving upwards.
 */
void move_up(bool value)
{
    if (value)
    {
        element_holder.player_bat.speed.y = BAT_SPEED;
    }
    else
    {
        element_holder.player_bat.speed.y = 0;
    }
}

/*
 * Function:  move_down (bool)
 * --------------------
 * Signals that you want to move the player bat downwards or stop moving downwards.
 */
void move_down(bool value)
{
    if (value)
    {
        element_holder.player_bat.speed.y = -BAT_SPEED;
    }
    else
    {
        element_holder.player_bat.speed.y = 0;
    }
}

/*
 * Function:  get_game_elements
 * --------------------
 * Retrieves all GameElements with their current state.
 */
struct GameElements get_game_elements(void) { return element_holder; }

/*
 * Function:  progress_time
 * --------------------
 * Tells the pong model to calculate the states for the next time step.
 */
void progress_time(bool two_players)
{
    struct Vector *position = &(element_holder.ball.position);
    struct Vector *speed = &(element_holder.ball.speed);

    if (!two_players)
        ai_routine();
    move_bats();
    collide_with_object(&(element_holder.ball), &(element_holder.player_bat));
    collide_with_object(&(element_holder.ball), &(element_holder.opponent_bat));
    progress_time_in_dimension(&(position->x), &(speed->x));
    progress_time_in_dimension(&(position->y), &(speed->y));
}

void progress_time_in_dimension(float *position_dimension, float *speed_dimension)
{
    if (*position_dimension + *speed_dimension <= FIELD_LIMIT &&
        *position_dimension + *speed_dimension >= -FIELD_LIMIT)
    {
        *position_dimension = *position_dimension + *speed_dimension;
    }
    if (*position_dimension + *speed_dimension >= FIELD_LIMIT ||
        *position_dimension + *speed_dimension <= -FIELD_LIMIT)
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
    };
}

void move_bat(struct MassiveObject *bat)
{
    if ((*bat).position.y + (*bat).speed.y <= FIELD_LIMIT &&
        (*bat).position.y + (*bat).speed.y >= -FIELD_LIMIT)
    {
        (*bat).position.y = (*bat).position.y + (*bat).speed.y;
    }
}

void move_bats()
{
    move_bat(&element_holder.player_bat);
    move_bat(&element_holder.opponent_bat);
}

/*
 * Function:  get_millis
 * --------------------
 * Retrieves a timestamp in ms
 */
long get_millis()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((long)ts.tv_sec * 1000000000L + ts.tv_nsec) / 1000000L;
}

long last_decided = 0L;
void ai_routine()
{
    long current_time = get_millis();
    if (200 < (current_time - last_decided)) // allow computer player a decision every 200 ms
    {
        last_decided = current_time;
        if (element_holder.ball.position.y >
            element_holder.opponent_bat.position.y + element_holder.opponent_bat.rectangle.y2)
        {
            element_holder.opponent_bat.speed.y = BAT_SPEED;
        }
        else if (element_holder.ball.position.y <
                 element_holder.opponent_bat.position.y + element_holder.opponent_bat.rectangle.y1)
        {
            element_holder.opponent_bat.speed.y = -BAT_SPEED;
        }
        else
        {
            element_holder.opponent_bat.speed.y = 0.0f;
        }
    }
}