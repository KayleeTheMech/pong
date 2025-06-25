#ifndef PONG_H_
#define PONG_H_
#include "structs.h"
#include <stdbool.h>

/*
 * Function:  move_up (bool)
 * --------------------
 * Signals that you want to move the player bat upwards or stop moving upwards.
 */
void move_up(bool value, bool player_one);
/*
 * Function:  move_down (bool)
 * --------------------
 * Signals that you want to move the player bat downwards or stop moving downwards.
 */
void move_down(bool value, bool player_one);

void tilt_bat(float tilt_angle, bool player_one);

/*
 * Function:  get_game_elements
 * --------------------
 * Retrieves all GameElements with their current state.
 */
struct GameElements get_game_elements(void);
/*
 * Function:  progress_time
 * --------------------
 * Tells the pong model to calculate the states for the next time step.
 */
void progress_time(bool two_players);

#endif
