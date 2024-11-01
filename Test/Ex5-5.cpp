#include <GL/glut.h>
#include <random>
#include <vector>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PI = 3.14159265359f;

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());

float randomFloat(float min, float max) {
   std::uniform_real_distribution<float> dis(min, max);
   return dis(gen);
}

// Previous CloudLayer and Cloud structs remain the same
struct CloudLayer {
   float x, y;
   float width, height;
   float density;
   float opacity;
   float particleSize;
};

struct Cloud {
   std::vector<CloudLayer> layers;
   float x, y;
   float width, height;
};

// Sun properties
struct Sun {
   float x = 0.15f;      // Sun position X
   float y = 0.85f;      // Sun position Y
   float radius = 0.08f; // Sun radius
   float brightness = 0.9f;
} sun;

// Light ray properties
struct LightRay {
   float startAngle = -45.0f;  // Angle for diagonal rays
   float raySpread = 30.0f;    // Spread angle of rays
   int numRays = 12;          // Number of light rays
   float maxLength = 2.0f;    // Length of rays
   float opacity = 0.3f;      // Ray opacity
} lightRays;

// Cloud definitions remain the same
std::vector<Cloud> clouds = {
   // ... (previous cloud definitions remain unchanged)
   {
       {
           {0.2f, 0.8f, 0.25f, 0.15f, 1.2f, 0.9f, 0.015f},
           {0.2f, 0.8f, 0.3f, 0.18f, 1.0f, 0.7f, 0.02f},
           {0.2f, 0.8f, 0.35f, 0.2f, 0.8f, 0.5f, 0.025f}
       },
       0.2f, 0.8f, 0.35f, 0.2f
   },
   {
       {
           {0.5f, 0.75f, 0.4f, 0.12f, 1.3f, 0.95f, 0.015f},
           {0.5f, 0.75f, 0.45f, 0.15f, 1.1f, 0.75f, 0.02f},
           {0.5f, 0.75f, 0.5f, 0.18f, 0.9f, 0.55f, 0.025f}
       },
       0.5f, 0.75f, 0.5f, 0.18f
   },
   {
       {
           {0.8f, 0.7f, 0.35f, 0.1f, 1.2f, 0.9f, 0.015f},
           {0.8f, 0.7f, 0.4f, 0.12f, 1.0f, 0.7f, 0.02f},
           {0.8f, 0.7f, 0.45f, 0.15f, 0.8f, 0.5f, 0.025f}
       },
       0.8f, 0.7f, 0.45f, 0.15f
   }
};

// Previous cloud drawing functions remain the same
void drawCloudParticle(float x, float y, float size) {
   float halfSize = size / 2.0f;
   glBegin(GL_QUADS);
   glVertex2f(x - halfSize, y - halfSize);
   glVertex2f(x + halfSize, y - halfSize);
   glVertex2f(x + halfSize, y + halfSize);
   glVertex2f(x - halfSize, y + halfSize);
   glEnd();
}

void drawCloudLayer(const CloudLayer& layer) {
   int numParticles = static_cast<int>(layer.width * layer.height * layer.density * 5000);

   for (int i = 0; i < numParticles; i++) {
       float x = randomFloat(layer.x - layer.width / 2, layer.x + layer.width / 2);
       float y = randomFloat(layer.y - layer.height / 2, layer.y + layer.height / 2);

       float dx = (x - layer.x) / (layer.width / 2);
       float dy = (y - layer.y) / (layer.height / 2);
       float distSq = dx * dx + dy * dy;

       if (distSq <= 1.0) {
           float alpha = layer.opacity * exp(-distSq * 2);
           glColor4f(1.0f, 1.0f, 1.0f, alpha);
           float sizeVar = randomFloat(0.8f, 1.2f);
           drawCloudParticle(x, y, layer.particleSize * sizeVar);
       }
   }
}

void drawCloud(const Cloud& cloud) {
   for (const auto& layer : cloud.layers) {
       drawCloudLayer(layer);
   }
}

// New function to draw sun
void drawSun() {
   // Draw sun glow
   glBegin(GL_TRIANGLE_FAN);
   glColor4f(1.0f, 1.0f, 0.8f, 0.3f);
   glVertex2f(sun.x, sun.y);
   float outerRadius = sun.radius * 1.5f;
   for (int i = 0; i <= 360; i += 10) {
       float angle = i * PI / 180.0f;
       glColor4f(1.0f, 1.0f, 0.8f, 0.0f);
       glVertex2f(sun.x + cos(angle) * outerRadius,
           sun.y + sin(angle) * outerRadius);
   }
   glEnd();

   // Draw sun core
   glBegin(GL_TRIANGLE_FAN);
   glColor4f(1.0f, 1.0f, 1.0f, sun.brightness);
   glVertex2f(sun.x, sun.y);
   for (int i = 0; i <= 360; i += 10) {
       float angle = i * PI / 180.0f;
       glVertex2f(sun.x + cos(angle) * sun.radius,
           sun.y + sin(angle) * sun.radius);
   }
   glEnd();
}

// New function to draw light rays
void drawLightRays() {
   float raySpacing = lightRays.raySpread / (lightRays.numRays - 1);
   float baseAngle = lightRays.startAngle - lightRays.raySpread / 2;

   glBegin(GL_TRIANGLES);
   for (int i = 0; i < lightRays.numRays; i++) {
       float angle = (baseAngle + i * raySpacing) * PI / 180.0f;
       float rayWidth = 0.02f + randomFloat(-0.005f, 0.005f);

       glColor4f(1.0f, 1.0f, 0.8f, lightRays.opacity);
       glVertex2f(sun.x, sun.y);

       float x1 = sun.x + cos(angle - rayWidth) * lightRays.maxLength;
       float y1 = sun.y + sin(angle - rayWidth) * lightRays.maxLength;
       float x2 = sun.x + cos(angle + rayWidth) * lightRays.maxLength;
       float y2 = sun.y + sin(angle + rayWidth) * lightRays.maxLength;

       glColor4f(1.0f, 1.0f, 0.8f, 0.0f);
       glVertex2f(x1, y1);
       glVertex2f(x2, y2);
   }
   glEnd();
}

void display() {
   glClear(GL_COLOR_BUFFER_BIT);

   // Draw gradient sky background
   glBegin(GL_QUADS);
   glColor3f(0.529f, 0.808f, 0.922f);
   glVertex2f(0.0f, 1.0f);
   glVertex2f(1.0f, 1.0f);
   glColor3f(0.39f, 0.58f, 0.93f);
   glVertex2f(1.0f, 0.0f);
   glVertex2f(0.0f, 0.0f);
   glEnd();

   // Enable blending
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Draw light rays first
   drawLightRays();

   // Draw sun
   drawSun();

   // Draw clouds
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

   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POLYGON_SMOOTH);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Sky Scene with Sun and Light Rays");

   init();
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}