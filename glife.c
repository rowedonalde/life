#include <stdio.h>
#include <time.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <unistd.h>

#include "lifelib.h"

// On my machine, this is in microseconds, but that may differ
// based on implementation, I think:
clock_t last_frame_time;

int width;
int height;

void init(void) {
    // Select clearing/background color:
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // Initialize viewing values:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void displayCallback(void) {
    // Clear all pixels:
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw randomly colored rectangle:
    glColor3f(
        (rand() % 100) / 100.0,
        (rand() % 100) / 100.0,
        (rand() % 100) / 100.0
    );
    glBegin(GL_POLYGON);
        glVertex3f(0.25, 0.25, 0.0);
        glVertex3f(0.75, 0.25, 0.0);
        glVertex3f(0.75, 0.75, 0.0);
        glVertex3f(0.25, 0.75, 0.0);
    glEnd();

    glFlush();
}

void idleAnimation(void) {
    if (clock() - last_frame_time >= 1000000) {
        glutPostRedisplay();
        last_frame_time = clock();
        printf("last_frame_time: %d\n", (int)last_frame_time);
    }
}

/*
 * Displays game graphically rather than with ASCII
 */
void gdisplay(int* board, int width, int height) {
    // Clear all pixels:
    glClear(GL_COLOR_BUFFER_BIT);
    
    // The living cells will be white:
    glColor3f(1.0, 1.0, 1.0);
}

int main(int argc, char** argv) {
    // Game setup:
    // The board state on even ticks:
    int* even = input_to_board(&width, &height);
    // The board state on odd ticks:
    int* odd = malloc(width * height * sizeof(int));
    blank(odd, width * height);
    
    // GL setup:
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Conway's Game of Life");
    init();
    glutDisplayFunc(displayCallback);
    glutIdleFunc(idleAnimation);
    clock();
    glutMainLoop();
    
    return 0;
}