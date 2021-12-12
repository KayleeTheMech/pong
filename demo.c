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
        .speed = {.x = 0.05f, .y = 0.01f},
    },
};

void progress_time()
{
    // x
    struct Vector *position=&(element_holder.ball.position);
    struct Vector *speed=&(element_holder.ball.speed);

    if (position->x + speed->x <=1.0f && position->x + speed->x >=-1.0f){
        // next position within bounds?
        position->x = position->x + speed->x;
    }
    if (position->x + speed->x >= 1.0f || position->x + speed->x <= -1.0f)
    {
        // no it hit the bounds, reflect the ball
        position->x = position->x - speed->x;
        speed->x = -speed->x;
    }    
    if (position->y + speed->y <=1.0f && position->y + speed->y >=-1.0f){
        // next position within bounds?
        position->y = position->y + speed->y;
    }
    if (position->y + speed->y >= 1.0f || position->y + speed->y <= -1.0f)
    {
        // no it hit the bounds, reflect the ball
        position->y = position->y - speed->y;
        speed->y = -speed->y;
    }
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
    glutTimerFunc(100, routine, 0);
}

int main(int argc, char *argv[])
{

    init(argc, argv);
    glutTimerFunc(25, routine, 0);
    glutDisplayFunc(init_display);
    glutMainLoop();
}