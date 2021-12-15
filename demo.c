/*  demo.c */
#include <stdbool.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "pong.h"
#include "structs.h"

const int MSEC_PER_FRAME = 1;
const float BALL_SIZE = 0.04f;
void game_loop(int t);

void key_pressed(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        // trigger up movement
        move_up(true);
    }
    else if (key == 's')
    {
        // trigger down movement
        move_down(true);
    }
}

void key_up(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        // end up movement
        move_up(false);
    }
    else if (key == 's')
    {
        // end down movement
        move_down(false);
    }
}

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1024, 1024);
    glutCreateWindow("Pong Test");
    glutKeyboardFunc(key_pressed);
    glutKeyboardUpFunc(key_up);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutTimerFunc(MSEC_PER_FRAME, game_loop, 0);
}

void reset_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRectf(-0.99f, 0.99f, 0.99f, -0.99f);
}

void paint_massive_object(struct MassiveObject object)
{
    glRectf(
        object.position.x + object.rectangle.x1,
        object.position.y + object.rectangle.y1,
        object.position.x + object.rectangle.x2,
        object.position.y + object.rectangle.y2);
}

void paint_objects(void)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    // paint ball
    glRectf(
        get_game_elements().ball.position.x - BALL_SIZE / 2,
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
    progress_time();
    // Update display
    paint_objects();
    glutPostRedisplay();
    glutSwapBuffers();
    // Reset timer
    glutTimerFunc(MSEC_PER_FRAME, game_loop, 0);
}

int main(int argc, char *argv[])
{
    init(argc, argv);
    glutDisplayFunc(reset_display);
    glutMainLoop();
}
