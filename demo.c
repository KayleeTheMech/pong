/*  demo.c */
#include "pong.h"
#include "structs.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef __GNUC__
#pragma STDC FENV_ACCESS ON
#endif

const int MSEC_PER_FRAME = 1;
const float BALL_SIZE = 0.04f;
const float MAX_TILT = 0.5f;
static bool two_player_flag = false;
static struct option long_options[] = {{"two-person", no_argument, 0, 't'}, {0, 0, 0, 0}};

void game_loop(int t);

void key_pressed(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        // trigger up movement
        move_up(true, true);
        break;
    case 's':
        // trigger down movement
        move_down(true, true);
        break;
    case 'a':
        tilt_bat(-MAX_TILT, true);
        break;
    case 'd':
        tilt_bat(MAX_TILT, true);
        break;
    default:
        break;
    }
}
void key_special_pressed(int key, int x, int y)
{
    if (key == GLUT_KEY_UP && two_player_flag)
    {
        // up arrow key pressed
        move_up(true, false);
    }
    else if (key == GLUT_KEY_DOWN && two_player_flag)
    {
        // down arrow key pressed
        move_down(true, false);
    }
}

void key_released(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        // end up movement
        move_up(false, true);
        break;
    case 's':
        // end down movement
        move_down(false, true);
        break;
    case 'a':
        tilt_bat(0, true);
        break;
    case 'd':
        tilt_bat(0, true);
        break;
    default:
        break;
    }
}

void key_special_released(int key, int x, int y)
{
    if (key == GLUT_KEY_UP && two_player_flag)
    {
        // up arrow key released
        move_up(false, false);
    }
    else if (key == GLUT_KEY_DOWN && two_player_flag)
    {
        // down arrow key released(
        move_down(false, false);
    }
}

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1024, 1024);
    glutCreateWindow("Pong Test");
    glutKeyboardFunc(key_pressed);
    glutKeyboardUpFunc(key_released);
    glutSpecialFunc(key_special_pressed);
    glutSpecialUpFunc(key_special_released);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutTimerFunc(MSEC_PER_FRAME, game_loop, 0);
}

void reset_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRectf(-0.99f, 0.99f, 0.99f, -0.99f);
}

void rotate_coordinates(float alpha, float *x, float *y)
{
    // || cos(alpha)   -sin(alpha) ||  | x |
    // || sin(alpha)   cos(alpha) ||  | y |
    *x = cosf(alpha) * *x - sinf(alpha) * *y;
    *y = sinf(alpha) * *x + cosf(alpha) * *y;
}

void paint_massive_object(struct MassiveObject object)
{
    glBegin(GL_POLYGON);

    float x1 = -object.rectangle.width / 2;
    float y1 = -object.rectangle.height / 2;
    rotate_coordinates(object.tilt, &x1, &y1);
    glVertex2f(object.position.x + x1, object.position.y + y1);
    float x2 = -object.rectangle.width / 2;
    float y2 = object.rectangle.height / 2;
    rotate_coordinates(object.tilt, &x2, &y2);
    glVertex2f(object.position.x + x2, object.position.y + y2);
    float x3 = object.rectangle.width / 2;
    float y3 = object.rectangle.height / 2;
    rotate_coordinates(object.tilt, &x3, &y3);
    glVertex2f(object.position.x + x3, object.position.y + y3);
    float x4 = object.rectangle.width / 2;
    float y4 = -object.rectangle.height / 2;
    rotate_coordinates(object.tilt, &x4, &y4);
    glVertex2f(object.position.x + x4, object.position.y + y4);
    glEnd();
}

void paint_objects(void)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    // paint ball
    glRectf(get_game_elements().ball.position.x - BALL_SIZE / 2,
            get_game_elements().ball.position.y - BALL_SIZE / 2,
            get_game_elements().ball.position.x + BALL_SIZE / 2,
            get_game_elements().ball.position.y + BALL_SIZE / 2);
    // paint player bat
    paint_massive_object(get_game_elements().player_bat);
    // paint player bat
    paint_massive_object(get_game_elements().opponent_bat);
}

void game_loop(int t)
{
    reset_display();
    // Progress Time
    progress_time(two_player_flag);
    // Update display
    paint_objects();
    glutPostRedisplay();
    glutSwapBuffers();
    // Reset timer
    glutTimerFunc(MSEC_PER_FRAME, game_loop, 0);
}

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt_long(argc, argv, "t", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 't':
            two_player_flag = true;
            break;

        default:
            printf("%s", opt);
            printf("No option requested");
            break;
        }
    }
    init(argc, argv);
    glutDisplayFunc(reset_display);
    glutMainLoop();
}
