#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

typedef struct Pong
{
    uint16_t position_x;
    uint16_t position_y;
    int16_t speed_x;
    int16_t speed_y;
} Pong;

typedef struct PlayingField
{
    uint16_t width;
    uint16_t height;
} PlayingField;

void step_time_on_pong(Pong *pong, PlayingField *field)
{
    int16_t next_x = pong->position_x + pong->speed_x;
    int16_t next_y = pong->position_y + pong->speed_y;
    // check if next x position is in playing field
    if (next_x >= 0 && next_x < field->width)
    {
        // yes just set next_x
        pong->position_x = next_x;
    }
    else
    {
        // no, change direction of speed_x
        pong->speed_x = -pong->speed_x;
        pong->position_x = pong->position_x + pong->speed_x;
    }

    // check if next y position is in playing field
    if (next_y >= 0 && next_y < field->width)
    {
        // yes just set next_y
        pong->position_y = next_y;
    }
    else
    {
        // no, change direction of speed_x
        pong->speed_y = -pong->speed_y;
        pong->position_y = pong->position_y + pong->speed_y;
    }
}
 
int main()
{
    PlayingField field = {.width = 1000, .height = 600};
    Pong pong = {.position_x = 0, .position_y = 0, .speed_x = 20, .speed_y = 10};
    struct timespec delta = {0 /*secs*/, 50 * 1000000 /*nanosecs*/};
    do
    {
        step_time_on_pong(&pong, &field);
        printf("%d, %d\n", pong.position_x, pong.position_y);
        nanosleep(&delta, &delta);
    } while (true);
    return 0;
}