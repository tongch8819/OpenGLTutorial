#include <GL/glut.h>
#include <math.h>
float angle = 0.0f;  
const float radius = 0.5f;  
const float squareSize = 0.1f;  
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    float x = radius * cos(angle * M_PI / 180.0f);
    float y = radius * sin(angle * M_PI / 180.0f);
    glTranslatef(x, y, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-squareSize, -squareSize);
        glVertex2f(squareSize, -squareSize);
        glVertex2f(squareSize, squareSize);
        glVertex2f(-squareSize, squareSize);
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}
void update(int value) {
    angle -= 2.0f;  
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("矩形公转");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}