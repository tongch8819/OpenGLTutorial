#include <GL/glut.h>
#include <random>
#include <vector>
#include <cmath>

// Previous basic definitions remain the same
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PI = 3.14159265359f;

std::random_device rd;
std::mt19937 gen(rd());

float randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

// Color structure for sunset colors
struct Color {
    float r, g, b, a;
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

// Define sunset color palette
struct SunsetPalette {
    std::vector<Color> colors = {
        Color(1.0f, 0.4f, 0.2f, 1.0f),    // Deep red
        Color(1.0f, 0.6f, 0.2f, 1.0f),    // Orange-red
        Color(1.0f, 0.8f, 0.3f, 1.0f),    // Golden yellow
        Color(0.95f, 0.7f, 0.4f, 1.0f),   // Peach
        Color(0.9f, 0.5f, 0.3f, 1.0f)     // Coral
    };

    Color getRandomColor(float baseOpacity) {
        int idx = static_cast<int>(randomFloat(0, colors.size() - 0.01f));
        Color c = colors[idx];
        c.a = baseOpacity;
        return c;
    }
} sunsetPalette;

// Previous struct definitions with added color properties
struct CloudLayer {
    float x, y;
    float width, height;
    float density;
    float opacity;
    float particleSize;
    float colorVariation;  // How much to vary the colors
};

struct Cloud {
    std::vector<CloudLayer> layers;
    float x, y;
    float width, height;
};

// Sun and light ray properties remain similar but with adjusted colors
struct Sun {
    float x = 0.15f;
    float y = 0.85f;
    float radius = 0.08f;
    float brightness = 0.9f;
    Color color = Color(1.0f, 0.85f, 0.6f, 1.0f);  // Warmer sun color
} sun;

struct LightRay {
    float startAngle = -45.0f;
    float raySpread = 30.0f;
    int numRays = 12;
    float maxLength = 2.0f;
    float opacity = 0.3f;
    Color color = Color(1.0f, 0.7f, 0.4f, 1.0f);  // Warmer ray color
} lightRays;

// Updated cloud definitions with color variation parameters
std::vector<Cloud> clouds = {
    {
        {
            {0.2f, 0.8f, 0.25f, 0.15f, 1.2f, 0.9f, 0.015f, 0.7f},
            {0.2f, 0.8f, 0.3f, 0.18f, 1.0f, 0.7f, 0.02f, 0.8f},
            {0.2f, 0.8f, 0.35f, 0.2f, 0.8f, 0.5f, 0.025f, 0.9f}
        },
        0.2f, 0.8f, 0.35f, 0.2f
    },
    {
        {
            {0.5f, 0.75f, 0.4f, 0.12f, 1.3f, 0.95f, 0.015f, 0.7f},
            {0.5f, 0.75f, 0.45f, 0.15f, 1.1f, 0.75f, 0.02f, 0.8f},
            {0.5f, 0.75f, 0.5f, 0.18f, 0.9f, 0.55f, 0.025f, 0.9f}
        },
        0.5f, 0.75f, 0.5f, 0.18f
    },
    {
        {
            {0.8f, 0.7f, 0.35f, 0.1f, 1.2f, 0.9f, 0.015f, 0.7f},
            {0.8f, 0.7f, 0.4f, 0.12f, 1.0f, 0.7f, 0.02f, 0.8f},
            {0.8f, 0.7f, 0.45f, 0.15f, 0.8f, 0.5f, 0.025f, 0.9f}
        },
        0.8f, 0.7f, 0.45f, 0.15f
    }
};

void drawCloudParticle(float x, float y, float size, const Color& color) {
    glColor4f(color.r, color.g, color.b, color.a);
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
            Color particleColor = sunsetPalette.getRandomColor(alpha);

            // Adjust color based on height for realistic sunset effect
            float heightFactor = y / 1.0f;  // Normalize height
            particleColor.r *= (1.0f + heightFactor * 0.2f);
            particleColor.g *= (1.0f + heightFactor * 0.1f);
            particleColor.b *= (0.8f + heightFactor * 0.2f);

            float sizeVar = randomFloat(0.8f, 1.2f);
            drawCloudParticle(x, y, layer.particleSize * sizeVar, particleColor);
        }
    }
}

void drawCloud(const Cloud& cloud) {
    for (const auto& layer : cloud.layers) {
        drawCloudLayer(layer);
    }
}

void drawSun() {
    // Draw sun glow with warmer colors
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.6f, 0.2f, 0.3f);  // Warm orange glow
    glVertex2f(sun.x, sun.y);
    float outerRadius = sun.radius * 1.5f;
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * PI / 180.0f;
        glColor4f(1.0f, 0.4f, 0.1f, 0.0f);  // Fade to warm red
        glVertex2f(sun.x + cos(angle) * outerRadius,
            sun.y + sin(angle) * outerRadius);
    }
    glEnd();

    // Draw sun core with warm white
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.95f, 0.8f, sun.brightness);
    glVertex2f(sun.x, sun.y);
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * PI / 180.0f;
        glVertex2f(sun.x + cos(angle) * sun.radius,
            sun.y + sin(angle) * sun.radius);
    }
    glEnd();
}

void drawLightRays() {
    float raySpacing = lightRays.raySpread / (lightRays.numRays - 1);
    float baseAngle = lightRays.startAngle - lightRays.raySpread / 2;

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < lightRays.numRays; i++) {
        float angle = (baseAngle + i * raySpacing) * PI / 180.0f;
        float rayWidth = 0.02f + randomFloat(-0.005f, 0.005f);

        // Warm-colored rays
        glColor4f(1.0f, 0.7f, 0.3f, lightRays.opacity);
        glVertex2f(sun.x, sun.y);

        float x1 = sun.x + cos(angle - rayWidth) * lightRays.maxLength;
        float y1 = sun.y + sin(angle - rayWidth) * lightRays.maxLength;
        float x2 = sun.x + cos(angle + rayWidth) * lightRays.maxLength;
        float y2 = sun.y + sin(angle + rayWidth) * lightRays.maxLength;

        glColor4f(1.0f, 0.5f, 0.2f, 0.0f);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw sunset sky gradient
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.4f, 0.2f);    // Orange-red at bottom
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glColor3f(0.2f, 0.4f, 0.7f);    // Blue at top
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawLightRays();
    drawSun();

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
    glutCreateWindow("Sunset Cloud Scene");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}