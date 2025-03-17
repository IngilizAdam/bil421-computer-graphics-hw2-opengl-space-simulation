//
// Perspective view of a color cube using LookAt() and Perspective()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//

#include "Angel.h"
#include "gameobjects.h"
#include "planet.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

// Viewing transformation parameters

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat dr = 5.0 * DegreesToRadians;

GLuint model;  // model-view matrix uniform shader variable location

// Projection transformation parameters

GLfloat  fovy = 120.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.1, zFar = 100.0;
GLuint  project; // projection matrix uniform shader variable location
GLdouble viewer[3] = { 1.5, 0.0, 0.0 };
//----------------------------------------------------------------------------

std::vector<Planet*> planets;

std::vector<GameObject*> gameObjects;

// OpenGL initialization
void init()
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
    glUseProgram(program);
    model = glGetUniformLocation(program, "model_view");
    project = glGetUniformLocation(program, "projection");

    gameObjects.push_back(new Planet(vec3(0.0, 1.0, 0.0), 0.5, vec4(1.0, 0.0, 0.0, 1.0), 40, program));
    gameObjects.push_back(new Planet(vec3(0.0, -1.0, 0.0), 0.5, vec4(0.0, 0.0, 1.0, 1.0), 40, program));
    gameObjects.push_back(new Planet(vec3(0.0, -1.0, 1.0), 0.5, vec4(1.0, 0.0, 1.0, 1.0), 40, program));

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* point4  eye( radius*sin(theta)*cos(phi),
            radius*sin(theta)*sin(phi),
            radius*cos(theta),
        1.0 );*/
    point4  eye(viewer[0], viewer[1], viewer[2], 1.0);
    point4  at(0.0, 0.0, 0.0, 1.0);
    vec4    up(0.0, 0.0, 1.0, 0.0);
    printv(eye);
    mat4  mv = LookAt(eye, at, up);
    glUniformMatrix4fv(model, 1, GL_TRUE, mv);

    mat4  p = Perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(project, 1, GL_TRUE, p);

	// update every game object's buffer
    for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->updateBuffers();
    }

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{



    if (key == 'x') viewer[0] -= 1.0;
    if (key == 'X') viewer[0] += 1.0;
    if (key == 'y') viewer[1] -= 1.0;
    if (key == 'Y') viewer[1] += 1.0;
    if (key == 'z') viewer[2] -= 1.0;
    if (key == 'Z') viewer[2] += 1.0;
    glutPostRedisplay();
}


//----------------------------------------------------------------------------

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    aspect = GLfloat(width) / height;
}

//----------------------------------------------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

    glutInitWindowSize(512, 512);
    //    glutInitContextVersion( 3, 2 );
      //  glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow("Color Cube");
    glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
