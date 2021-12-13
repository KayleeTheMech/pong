/*  demo.c */
#include <stdbool.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

const float FIELD_LIMIT = 1.0f;
const float BALL_SIZE = 0.04f;
const float BAT_SPEED = 0.05f;
bool move_player_bat_up = false;
bool move_player_bat_down = false;

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
    struct InertialObject ball;
};

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

void progress_time_in_dimension(float *position_dimension, float *speed_dimension)
{
    if (*position_dimension + *speed_dimension <= FIELD_LIMIT && *position_dimension + *speed_dimension >= -FIELD_LIMIT)
    {
        // next position within bounds?
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

void progress_time()
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

void keyPressed(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        // trigger up movement
        move_player_bat_up = true;
    }
    else if (key == 's')
    {
        // trigger down movement
        move_player_bat_down = true;
    }
}

void keyUp(unsigned char key, int x, int y)
{
    if (key == 'w')
    {
        // end up movement
        move_player_bat_up = false;
    }
    else if (key == 's')
    {
        // end down movement
        move_player_bat_down = false;
    }
}

void setup()
{
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1024, 1024);
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
    // paint ball
    glRectf(
        element_holder.ball.position.x - BALL_SIZE / 2,
        element_holder.ball.position.y - BALL_SIZE / 2,
        element_holder.ball.position.x + BALL_SIZE / 2,
        element_holder.ball.position.y + BALL_SIZE / 2);
    // paint player bat
    glRectf(
        element_holder.player_bat.position.x + element_holder.player_bat.rectangle.x1,
        element_holder.player_bat.position.y + element_holder.player_bat.rectangle.y1,
        element_holder.player_bat.position.x + element_holder.player_bat.rectangle.x2,
        element_holder.player_bat.position.y + element_holder.player_bat.rectangle.y2);
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