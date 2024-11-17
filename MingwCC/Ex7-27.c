#include <GL/glut.h>
#include <math.h>

float angle = 0.0f;  // Current angle of rotation
const float radius = 0.5f;  // Radius of the circular path
const float squareSize = 0.1f;  // Size of the square

void init() {
    // Set white background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Save the current matrix state
    glPushMatrix();
    
    // Move to the center of the window
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    // Calculate the position on the circular path
    float x = radius * cos(angle * M_PI / 180.0f);
    float y = radius * sin(angle * M_PI / 180.0f);
    
    // Translate to the current position on the circle
    glTranslatef(x, y, 0.0f);
    
    // Set color to black for the square
    glColor3f(0.0f, 0.0f, 0.0f);
    
    // Draw the square
    glBegin(GL_QUADS);
        glVertex2f(-squareSize, -squareSize);
        glVertex2f(squareSize, -squareSize);
        glVertex2f(squareSize, squareSize);
        glVertex2f(-squareSize, squareSize);
    glEnd();
    
    // Restore the matrix state
    glPopMatrix();
    
    glutSwapBuffers();
}

void update(int value) {
    // Update angle (decrease for clockwise rotation)
    angle -= 2.0f;  // Adjust this value to control rotation speed
    
    // Keep angle in [0, 360) range
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    
    // Trigger redisplay
    glutPostRedisplay();
    
    // Set the next timer
    glutTimerFunc(16, update, 0);  // ~60 FPS
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Maintain aspect ratio
    if (w <= h) {
        gluOrtho2D(-1.0, 1.0, -1.0 * h / w, 1.0 * h / w);
    } else {
        gluOrtho2D(-1.0 * w / h, 1.0 * w / h, -1.0, 1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Square on Circular Path");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    
    glutMainLoop();
    return 0;
}