#include <iostream>
#include <vector>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

using namespace std;

// Global color indicators
#define GREY	0
#define RED	    1
#define BLACK	2
#define YELLOW  3
#define MAGENTA 4

// Position of light
float light_position[4] = {-1, 3, 0, 1.0};

// Ambience, diffusion and specular properties of light
float ambient[4] = {0.2, 0.2, 0.2, 1.0};
float diffuse[4] = {0.8, 0.8, 0.8, 1.0};
float specular[4] = {0.4, 0.4, 0.4, 1.0};

// Ground, back and wall coordinates
float ground[4] = {0.0, 1.0, 0.0, 1.499};
float back[4] = {0.0, 0.0, 1.0, 0.899};

// Shadow matrix
unsigned char shadow[128] = {
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55
};

// Color instances to indicators
float colors[5][4] = {
    {0.8, 0.8, 0.8, 1.0},
    {0.8, 0.0, 0.0, 1.0},
    {0.0, 0.0, 0.0, 0.6},
    {0.8, 0.8, 0.0, 1.0},
    {0.8, 0.0, 0.8, 1.0},
};

GLuint texture;
GLUquadric* sphere;

void setColorOfMaterial(int color)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &colors[color][0]);
}

void createTex(void)
{
    unsigned char data[256][256][3];
    for (int i = 0; i < 255; i++) {
        for (int j = 0; j < 255; j++) {
            unsigned char *p = data[i][j];
            p[0] = p[1] = p[2] = (j * i + 3) & 8 ? 255 : 0;
        }
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *) data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void createSphere(int color)
{
    // Set color of the spheres
    setColorOfMaterial(color);

    // Plastic sphere
    glTranslatef(2, -1.5, 0);
    glutSolidSphere(1, 128, 64);
    glTranslatef(-2, 1.5, 0);

    // Create texture
    createTex();
    // Initialize new sphere
    sphere = gluNewQuadric();
    // Textured sphere
    glEnable(GL_TEXTURE_2D);
    gluQuadricDrawStyle(sphere, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    glTranslatef(-2, -1.5, 0);
    gluSphere(sphere, 1, 128, 64);
    glTranslatef(2, 1.5, 0);
    glDisable(GL_TEXTURE_2D);
}

void createScene(int w, int h, int color)
{
    static int initialized = 0;
    static GLuint checklist = 0;
    if (!initialized) {
        // Normal of the square plane
        static float square_normal[4] = {0.0, 0.0, 1.0, 0.0};
        // Square plane
        static float square[4][4];
        int i, j;
        if (!checklist) {
            checklist = glGenLists(1);
        }
        glNewList(checklist, GL_COMPILE_AND_EXECUTE);
        glNormal3fv(square_normal);
        glBegin(GL_QUADS);
        // Rendering square plane
        for (j = 0; j < h; ++j) {
            for (i = 0; i < w; ++i) {
                square[0][0] = -1.0 + 2.0 / w * i;
                square[0][1] = -1.0 + 2.0 / h * (j + 1);
                square[0][2] = 0.0;
                square[0][3] = 1.0;

                square[1][0] = -1.0 + 2.0 / w * i;
                square[1][1] = -1.0 + 2.0 / h * j;
                square[1][2] = 0.0;
                square[1][3] = 1.0;

                square[2][0] = -1.0 + 2.0 / w * (i + 1);
                square[2][1] = -1.0 + 2.0 / h * j;
                square[2][2] = 0.0;
                square[2][3] = 1.0;

                square[3][0] = -1.0 + 2.0 / w * (i + 1);
                square[3][1] = -1.0 + 2.0 / h * (j + 1);
                square[3][2] = 0.0;
                square[3][3] = 1.0;

                setColorOfMaterial(color);
                glVertex4fv(&square[0][0]);
                glVertex4fv(&square[1][0]);
                glVertex4fv(&square[2][0]);
                glVertex4fv(&square[3][0]);
            }
        }
        glEnd();
        glEndList();
        initialized = 1;
    } else {
        glCallList(checklist);
    }
}

void adjustMatrix(float ground[4], float light[4])
{
    float dot;
    float shadowMat[4][4];

    dot = ground[0] * light[0] + ground[1] * light[1] + ground[2] * light[2] + ground[3] * light[3];

    shadowMat[0][0] = dot - light[0] * ground[0];
    shadowMat[1][0] = 0.0 - light[0] * ground[1];
    shadowMat[2][0] = 0.0 - light[0] * ground[2];
    shadowMat[3][0] = 0.0 - light[0] * ground[3];

    shadowMat[0][1] = 0.0 - light[1] * ground[0];
    shadowMat[1][1] = dot - light[1] * ground[1];
    shadowMat[2][1] = 0.0 - light[1] * ground[2];
    shadowMat[3][1] = 0.0 - light[1] * ground[3];

    shadowMat[0][2] = 0.0 - light[2] * ground[0];
    shadowMat[1][2] = 0.0 - light[2] * ground[1];
    shadowMat[2][2] = dot - light[2] * ground[2];
    shadowMat[3][2] = 0.0 - light[2] * ground[3];

    shadowMat[0][3] = 0.0 - light[3] * ground[0];
    shadowMat[1][3] = 0.0 - light[3] * ground[1];
    shadowMat[2][3] = 0.0 - light[3] * ground[2];
    shadowMat[3][3] = dot - light[3] * ground[3];

    glMultMatrixf((const GLfloat *) shadowMat);
}

void display()
{
    GLfloat cubeXform[4][4];

    // Clear window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw ground
    glPushMatrix();
    glTranslatef(0.0, -1.5, 0.0);
    glRotatef(-90.0, 1, 0, 0);
    glScalef(2.0, 2.0, 2.0);

    createScene(6, 6, YELLOW);
    glPopMatrix();

    // Draw back
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.9);
    glScalef(2.0, 2.0, 2.0);

    createScene(6, 6, YELLOW);
    glPopMatrix();

    // Draw roof
    glPushMatrix();
  	glTranslatef(0.0, 1.5, 0.0);
  	glRotatef(90.0, 1, 0, 0);
  	glScalef(2.0, 2.0, 2.0);

  	createScene(6, 6, GREY);
  	glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    glScalef(0.3, 0.3, 0.3);

    glScalef(1.0, 2.0, 1.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) cubeXform);

    // Draw spheres
    createSphere(MAGENTA);
    glPopMatrix();

    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);

    // Set shadow matrix for ground
    glPushMatrix();
    adjustMatrix(ground, light_position);
    glTranslatef(0.0, 0.0, 2.0);
    glMultMatrixf((const GLfloat *) cubeXform);

    // Render ground shadows on scene
    createSphere(BLACK);
    glPopMatrix();

    // Set shadow matrix for back
    glPushMatrix();
    adjustMatrix(back, light_position);
    glTranslatef(0.0, 0.0, 2.0);
    glMultMatrixf((const GLfloat *) cubeXform);

    // Render back shadows on scene
    createSphere(BLACK);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    // Initialize window
    glutInitWindowSize(1280, 720);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Assignment 3");

    glutDisplayFunc(display);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -2.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_NORMALIZE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonStipple((const GLubyte *) shadow);

    glClearColor(0.8, 0.8, 0.8, 1);
    glClearIndex(0);
    glClearDepth(1);

    glutMainLoop();
}
