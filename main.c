// Jared Rand
// 500683609

/*******************************************************************
           Multi-Part Model Construction and Manipulation
********************************************************************/

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <glut/glut.h>
#else
    #include <gl/glut.h>
#endif
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Vector3D.h"
#include "QuadMesh.h"
#include "Submarine.h"
#include "Scenery.h"
#include "Mountain.h"
#include "Ground.h"
#include "Torpedo.h"
#include "Camera.h"
#include "Util.h"
#include <stdbool.h>

const int meshSize = 64;    // Default Mesh Size
int vWidth = 650;     // Viewport width in pixels
int vHeight = 500;    // Viewport height in pixels

bool useShader = false;
int shaderProgram;
static int currentButton;
static unsigned char currentKey;

// Lighting/shading and material properties for submarine - upcoming lecture - just copy for now

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat submarine_mat_ambient[] = { 0.4F, 0.4F, 0.4F, 1.0F };
static GLfloat submarine_mat_specular[] = { 0.5F, 0.5F, 0.5F, 1.0F };
static GLfloat submarine_mat_diffuse[] = { 0.1F, 0.1F, 0.1F, 1.0F };
static GLfloat submarine_mat_shininess[] = { 0.75F };

Camera* camera;

// A quad mesh representing the ground
static QuadMesh groundMesh;

// A pointer to the submarine struct
Submarine* submarine;

// A pointer to the submarine struct
Submarine* enemySub;

// A pointer to the torpedo struct
Torpedo* torpedo;

// A pointer to the scenery struct
Scenery* scenery;

// A stack of mountains
MountainStack* mountainStack = NULL;

Texture* textures[5];

// A timer to keep track of time elapsed since last frame
// to support smooth movement.
int oldTimeSinceStart = 0;

int timerStart = 0;
int timerEnd = 0;

// Structure defining a bounding box, currently unused
struct BoundingBox {
   Vector3D min;
   Vector3D max;
} BBox;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardRelease(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void functionKeysRelease(int key, int x, int y);
void update();
Vector3D ScreenToWorld(int x, int y);
void PrintHelpText();
char* readFileToString(char* filename);
int loadAndCompileShader(char* filename, int shaderType);


int main(int argc, char **argv)
{
    // Code to enable printing to stdout
    // while the program is running
    setvbuf (stdout, NULL, _IONBF, 0);

    camera = InitializeCamera();

    // Initialize the mountains
    mountainStack = initMountainStack(4);

    // Add some mountains
    pushMountainStack(mountainStack, (Mountain) {
        1, 17,
        -1, 54,
    });
    pushMountainStack(mountainStack, (Mountain) {
        -16, -33,
        3, 10,
    });
    pushMountainStack(mountainStack, (Mountain) {
        -20, 10,
        -6, 30,
    });
    pushMountainStack(mountainStack, (Mountain) {
        23, 36,
        4, 43,
    });
    pushMountainStack(mountainStack, (Mountain) {
        55, 46,
        1, 20,
    });


    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 3");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

    // Create and attach the shader program
    shaderProgram = glCreateProgram();
    int vertexShader = loadAndCompileShader("vertex.glsl", GL_VERTEX_SHADER);
    int fragmentShader = loadAndCompileShader("fragment.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardRelease);
    glutSpecialFunc(functionKeys);
    glutSpecialUpFunc(functionKeysRelease);
    glutIdleFunc(update);

    // Start event loop, never returns
    glutMainLoop();

    return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Initialize Textures
    textures[0] = generateTexture("ocean-floor.png", GL_TEXTURE0);
    textures[1] = generateTexture("column.jpg", GL_TEXTURE1);
    textures[2] = generateTexture("goodsub.jpg", GL_TEXTURE2);
    textures[3] = generateTexture("badsub.jpg", GL_TEXTURE3);
    textures[4] = generateTexture("torpedo.jpg", GL_TEXTURE4);

    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);   // This light is currently off

    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
    glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

    // Set up ground quad mesh
    // Make the mesh size 128
    int meshViewingSize = meshSize * 2;
    Vector3D origin = NewVector3D(-(meshViewingSize / 2), 0.0f, meshViewingSize / 2);
    Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
    Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
    groundMesh = NewQuadMesh(meshSize, textures[0]);
    InitMeshQM(&groundMesh, meshSize, origin, meshViewingSize, meshViewingSize, dir1v, dir2v);

    Vector3D ambient = NewVector3D(0.0f, 0.00f, 0.05f);
    Vector3D diffuse = NewVector3D(0.4f, 0.7f, 0.8f);
    Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
    SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);

    // Set up the bounding box of the scene
    // Currently unused. You could set up bounding boxes for your objects eventually.
    Set(&BBox.min, -(meshViewingSize / 2), 0.0, meshViewingSize / 2);
    Set(&BBox.max, meshViewingSize / 2, 6.0,  meshViewingSize / 2);

    // Initialize our submarine object
    submarine = InitializeSubmarine(0, 10, 0, textures[2]);

    // Initialize our submarine object
    enemySub = InitializeSubmarine(-15, 10, 0, textures[3]);

    // Initialize our scenery object
    scenery = InitializeScenery(textures[1]);
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (submarine != NULL && submarine->periscope.lookThroughPeriscope) {
        gluPerspective(submarine->periscope.zoom, (GLdouble)vWidth / vHeight, 0.2, 128);
    } else {
        gluPerspective(60.0, (GLdouble)vWidth / vHeight, 0.2, 128.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // M = I

    LookThroughCamera(camera, vWidth, vHeight);

    // Draw submarine

    // Set submarine material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, submarine_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, submarine_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, submarine_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, submarine_mat_shininess);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    // M is the current Model-View Matrix.
    //
    // Submarine is contains has its own push/pop matrix to keep its
    // context from manipulating the rest of the world
    RenderSubmarine(submarine); // M = I * V

    RenderSubmarine(enemySub); // M = I * V

    RenderTorpedo(torpedo);

    RenderScenery(scenery); // M = I * V

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    // Draw ground mesh
    DrawMeshQM(&groundMesh, meshSize); // M = I * V

    glutSwapBuffers();   // Double buffering, swap buffers
}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    vWidth = w;
    vHeight = h;
    // Set up viewport, projection, then change to modelview matrix mode - 
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (GLdouble)w / h, 0.2, 128.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // M = I

    LookThroughCamera(camera, w, h);
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    SubmarineHandleKeyPress(key, submarine);
}

// Callback, handles key releases from the keyboard, non-arrow keys
void keyboardRelease(unsigned char key, int x, int y) {
    torpedo = SubmarineHandleKeyRelease(key, submarine, textures[4]);
    switch (key) {
        case 'u':
            if (!useShader) {
                glUseProgram(shaderProgram);
            } else {
                glUseProgramObjectARB(0);
            }

            useShader = !useShader;
        break;
    }
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    // Help key
    if (key == GLUT_KEY_F1)
    {
        PrintHelpText();
    } else {
        SubmarineHandleFunctionKeyPress(key, submarine);
    }
}

// Callback, handles key releases from the function and arrow keys
void functionKeysRelease(int key, int x, int y) {
    SubmarineHandleFunctionKeyRelease(key, submarine);
}

void update() {
    int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    int delta = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;

    if (timerEnd - timeSinceStart <= 0) {
        timerStart = glutGet(GLUT_ELAPSED_TIME);
        timerEnd = timerStart + randomRange(600, 1500);

        RandomlyControlSubmarine(enemySub);
    }

    int xMin = -meshSize;
    int xMax = meshSize;
    int zMin = -meshSize;
    int zMax = meshSize;

    glLoadIdentity(); // M = I

    LookThroughCamera(camera, vWidth, vHeight);

    getAllVertexHeights(&groundMesh, mountainStack);

    submarine = SubmarineCheckCollisions(submarine, torpedo, &groundMesh);

    enemySub = SubmarineCheckCollisions(enemySub, torpedo, &groundMesh);

    if (enemySub == NULL) {
        enemySub = InitializeSubmarine(
            randomRange(-64, 64),
            randomRange(4, 30),
            randomRange(-64, 64),
            textures[3]
        );
    }

    UpdateSubmarine(enemySub, delta, xMin, xMax, zMin, zMax);

    UpdateSubmarine(submarine, delta, xMin, xMax, zMin, zMax);

    UpdateTorpedo(torpedo, delta);

    GetCamera(camera, submarine);

    glutPostRedisplay();
}


Vector3D ScreenToWorld(int x, int y)
{
    // you will need to finish this if you use the mouse
    return NewVector3D(0, 0, 0);
}

void PrintHelpText() {
    printf("%s", readFileToString("README.txt"));
}

char* readFileToString(char* filename) {
    char* buffer = 0;
    FILE* file = fopen(filename, "rb");

    if (file) {
        fseek(file, 0, SEEK_END);
        long length = sizeof(char) * ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc(length);

        if (buffer) {
            fread(buffer, 1, length, file);
            buffer[length] = '\0';
        }

        fclose(file);
    }

    return buffer;
}

int loadAndCompileShader(char* filename, int shaderType) {
    int handler = glCreateShader(shaderType);

    const char* shaderCode = readFileToString(filename);
    int length = strlen(shaderCode);
    const char* const* shaderCodeArray = { &shaderCode };

    glShaderSource(handler, 1, shaderCodeArray, &length);

    glCompileShader(handler);

    GLint shaderStatus = 0;

    glGetShaderiv(handler, GL_COMPILE_STATUS, &shaderStatus);

    if (shaderStatus == GL_FALSE) {
        printf("Failed to compile shader %s!\n", filename);

        GLint logLength = 0;
        glGetShaderiv(handler, GL_INFO_LOG_LENGTH, &logLength);

        char errorLog[logLength];
        glGetShaderInfoLog(handler, logLength, &logLength, &errorLog[0]);

        // Print the error log from the shadercode
        printf("%s\n", errorLog);

        // Prevent shader leaking
        glDeleteShader(handler);
    } else {
        printf("Successfully compiled shader %s!\n", filename);
    }

    return handler;
}
