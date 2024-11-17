#include <GL/glut.h>
#include <random>
#include <ctime>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());

// Function to generate random float between min and max
float randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

// Structure to represent a cloud region
struct CloudRegion {
    float x, y;           // Center position
    float width, height;  // Dimensions
    float density;        // Point density
    float grayLevel;      // Brightness (0.0 to 1.0)
    float pointSize;      // Size of points
};

// Array of cloud regions
CloudRegion clouds[] = {
    {0.2f, 0.7f, 0.3f, 0.2f, 0.8f, 0.9f, 2.0f},  // Light region
    {0.5f, 0.8f, 0.4f, 0.15f, 0.9f, 0.8f, 1.5f}, // Very light region
    {0.7f, 0.75f, 0.25f, 0.2f, 0.7f, 0.6f, 3.0f},// Medium region
    {0.4f, 0.65f, 0.3f, 0.18f, 0.6f, 0.4f, 4.0f} // Dark region
};

void drawCloud(const CloudRegion& cloud) {
    int numPoints = static_cast<int>(cloud.width * cloud.height * cloud.density * 1000);
    glPointSize(cloud.pointSize);
    glColor3f(cloud.grayLevel, cloud.grayLevel, cloud.grayLevel);

    glBegin(GL_POINTS);
    for (int i = 0; i < numPoints; i++) {
        float x = randomFloat(cloud.x - cloud.width / 2, cloud.x + cloud.width / 2);
        float y = randomFloat(cloud.y - cloud.height / 2, cloud.y + cloud.height / 2);

        // Only draw points that fall within an elliptical shape
        float normalizedX = (x - cloud.x) / (cloud.width / 2);
        float normalizedY = (y - cloud.y) / (cloud.height / 2);
        if (normalizedX * normalizedX + normalizedY * normalizedY <= 1.0) {
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void display5_5() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw blue sky background
    glBegin(GL_QUADS);
    glColor3f(0.529f, 0.808f, 0.922f); // Sky blue color
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    // Enable point smoothing for better looking points
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw all cloud regions
    for (const auto& cloud : clouds) {
        drawCloud(cloud);
    }

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cloud Scene");

    init();
    glutDisplayFunc(display5_5);
    glutMainLoop();
    return 0;
}