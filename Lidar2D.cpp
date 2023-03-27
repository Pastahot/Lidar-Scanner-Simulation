#include <GL/glut.h>
#include <cmath>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <vector>

struct Wall {
    float x1, y1, x2, y2, height;

    Wall(float x1, float y1, float x2, float y2, float height) : x1(x1), y1(y1), x2(x2), y2(y2), height(height) {}
};



std::vector<Wall> walls = {
    Wall(100, 100, 700, 100, 10),
    Wall(700, 100, 700, 500, 10),
    Wall(700, 500, 100, 500, 10),
    Wall(100, 500, 100, 100, 10),
    Wall(250, 100, 250, 400, 10),
    Wall(400, 200, 400, 500, 10),
    Wall(550, 100, 550, 400, 10),
};


const int windowWidth = 800;
const int windowHeight = 600;
const float laserLength = 200.0f;

int mouseX = 0;
int mouseY = 0;

std::pair<float, float> intersectsWall(float x1, float y1, float x2, float y2) {
    std::pair<float, float> closestIntersection(-1.0f, -1.0f);
    float closestDistance = std::numeric_limits<float>::max();

    for (const Wall& wall : walls) {
        float denominator = ((wall.y2 - wall.y1) * (x2 - x1)) - ((wall.x2 - wall.x1) * (y2 - y1));

        if (denominator == 0) {
            continue;
        }

        float ua = (((wall.x2 - wall.x1) * (y1 - wall.y1)) - ((wall.y2 - wall.y1) * (x1 - wall.x1))) / denominator;
        float ub = (((x2 - x1) * (y1 - wall.y1)) - ((y2 - y1) * (x1 - wall.x1))) / denominator;

        if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1) {
            float intersectionX = x1 + ua * (x2 - x1);
            float intersectionY = y1 + ua * (y2 - y1);
            float distance = std::sqrt((intersectionX - x1) * (intersectionX - x1) + (intersectionY - y1) * (intersectionY - y1));

            if (distance < closestDistance) {
                closestDistance = distance;
                closestIntersection = std::make_pair(intersectionX, intersectionY);
            }
        }
    }

    return closestIntersection;
}







void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

            //wall
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    for (const Wall& wall : walls) {
        glVertex2f(wall.x1, wall.y1);
        glVertex2f(wall.x2, wall.y2);
        glVertex2f(wall.x2, wall.y2 + wall.height);
        glVertex2f(wall.x1, wall.y1 + wall.height);
    }
    glEnd();


            //laser
    glColor3f(1.0f, 2.0f, 0.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 360; ++i) {
        float angle = 2.0f * static_cast<float>(M_PI) * i / 88.0f;  // beam count 100 opt
        float dx = laserLength * std::cos(angle);
        float dy = laserLength * std::sin(angle);

        float endPointX = mouseX + dx;
        float endPointY = (windowHeight - mouseY) + dy;

            //wall interaction
        std::pair<float, float> intersection = intersectsWall(mouseX, windowHeight - mouseY, endPointX, endPointY);
        if (intersection.first != -1.0f && intersection.second != -1.0f) {
            endPointX = intersection.first;
            endPointY = intersection.second;
        }

        glVertex2f(mouseX, windowHeight - mouseY);
        glVertex2f(endPointX, endPointY);
    }
    glEnd();


    glutSwapBuffers();
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouseMove(int x, int y) {
    mouseX = x;
    mouseY = y;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("LiDAR Simulation");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMove);

    glutMainLoop();
    return 0;
}
