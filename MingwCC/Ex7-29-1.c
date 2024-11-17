#include <GL/glut.h>
#include <math.h>

float pathAngle = 0.0f;  // Angle for position on circular path
float squareRotation = 0.0f;  // Angle for square's rotation
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
    float x = radius * cos(pathAngle * M_PI / 180.0f);
    float y = radius * sin(pathAngle * M_PI / 180.0f);
    
    // Translate to the current position on the circle
    glTranslatef(x, y, 0.0f);
    
    // Rotate the square around its center
    // squareRotation is synchronized with pathAngle (4x faster)
    glRotatef(squareRotation, 0.0f, 0.0f, 1.0f);
    
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

void idle() {
    // Update path angle (decrease for clockwise movement)
    pathAngle -= 1.0f;
    
    // Update square rotation (decrease for clockwise rotation)
    // Multiply by 4 to complete one rotation per quarter circle
    squareRotation = pathAngle * 4.0f;
    
    // Keep angles in reasonable ranges
    if (pathAngle < 0.0f) {
        pathAngle += 360.0f;
    }
    
    // Trigger redisplay
    glutPostRedisplay();

}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("自转加公转");
    
    init();
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    
    glutMainLoop();
    return 0;
}