/*  demo.c */
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

struct Vector
{
    float x;
    float y;
};

struct Object
{
    struct Vector position;
    struct Vector speed;
};

struct GameElements
{
    struct Object ball;
};

struct GameElements element_holder = {
    .ball = {
        .position = {.x = 0.0f, .y = 0.0f},
        .speed = {.x = 0.01f, .y = 0.001f},
    },
};

void progress_time_in_dimension(float *position_dimension, float *speed_dimension){

    if (*position_dimension + *speed_dimension <=1.0f && *position_dimension + *speed_dimension >=-1.0f){
        // next position within bounds?
        *position_dimension = *position_dimension + *speed_dimension;
    }
    if (*position_dimension + *speed_dimension >= 1.0f || *position_dimension + *speed_dimension <= -1.0f)
    {
        // no it hit the bounds, reflect the ball
        *position_dimension = *position_dimension -*speed_dimension;
        *speed_dimension = -*speed_dimension;
    }
}

void progress_time()
{
    // x
    struct Vector *position=&(element_holder.ball.position);
    struct Vector *speed=&(element_holder.ball.speed);

    progress_time_in_dimension(&(position->x), &(speed->x));
    progress_time_in_dimension(&(position->y), &(speed->y));
}

void setup()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Pong Test");
    setup();
}

void init_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glRectf(-0.99f, 0.99f, 0.99f, -0.99f);
}

void routine(int t)
{
    init_display();
    // Progress Time
    progress_time();
    // Update display
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(element_holder.ball.position.x - 0.01f, element_holder.ball.position.y - 0.01f, element_holder.ball.position.x + 0.01f, element_holder.ball.position.y + 0.01f);
    glutPostRedisplay();
    glutSwapBuffers();
    // Reset timer
    glutTimerFunc(1, routine, 0);
}

int main(int argc, char *argv[])
{

    init(argc, argv);
    glutTimerFunc(1, routine, 0);
    glutDisplayFunc(init_display);
    glutMainLoop();
}