#include "first.h"
#include "Angel.h"
#include "gameobjects.h"
#include "planet.h"
#include "spaceship.h"

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
GLfloat  zNear = 0.1, zFar = 10000.0;
GLuint  viewProjection; // projection matrix uniform shader variable location
GLdouble viewer[3] = { 0, 0, 150 };
//----------------------------------------------------------------------------

std::vector<GameObject*> gameObjects;

GameObject* spaceship;

// OpenGL initialization
void init()
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
    glUseProgram(program);
    model = glGetUniformLocation(program, "model");
    viewProjection = glGetUniformLocation(program, "view_projection");

	// create spaceship
	spaceship = new Spaceship(vec3(105, 0, 15), vec3(0, 0, 0), 1.0f, 0.2f, vec4(1, 0, 0, 1), 50, program);
	spaceship->setVelocity(vec4(0.0, -1.0, 0.0, 0.0));
	gameObjects.push_back(spaceship);

	// create planets
    int planetResolution = 40;
    float planetRadius = 1.0f;
    gameObjects.push_back(new Planet(vec3(30, 170, 15), vec3(0, 0, 0), planetRadius, vec4(1.00, 0.00, 0.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(80, 110, 25), vec3(0, 0, 0), planetRadius, vec4(0.00, 1.00, 0.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(70, 60, 12), vec3(0, 0, 0), planetRadius, vec4(0.00, 0.00, 1.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(90, 150, 13), vec3(0, 0, 0), 0.5, vec4(1.00, 1.00, 0.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(120, 80, 17), vec3(0, 0, 0), 0.5, vec4(1.00, 0.00, 1.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(150, 40, 15), vec3(0, 0, 0), 0.5, vec4(0.00, 1.00, 1.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(160, 170, 22), vec3(0, 0, 0), 0.5, vec4(1.00, 1.00, 1.00, 1.0), planetResolution, program));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get spaceship position and set camera over it
	vec4 spaceshipPosition = spaceship->getPosition();
	vec4 spaceshipVelocity = spaceship->getVelocity();
	vec4 offset = normalize(spaceshipVelocity) * THIRD_PERSON_BACK;
	vec4 position = spaceshipPosition - offset;
    vec4 leverage = vec4(0, 0, THIRD_PERSON_LEVERAGE, 0);
	viewer[0] = position.x;
	viewer[1] = position.y;
	viewer[2] = position.z + THIRD_PERSON_LEVERAGE;

    point4  eye(viewer[0], viewer[1], viewer[2], 1.0);
	// look at the direction of spaceship
    point4 at(spaceshipPosition + leverage);
    vec4    up(0.0, 0.0, 1.0, 0.0);
    mat4  viewMatrix = LookAt(eye, at, up);
    mat4  projectionMatrix = Perspective(fovy, aspect, zNear, zFar);
	mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(viewProjection, 1, GL_TRUE, viewProjectionMatrix);

	// update every game object's buffer
    for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->updateBuffers();
    }

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y) {
    if (key == 'x') viewer[0] -= 1.0;
    if (key == 'X') viewer[0] += 1.0;
    if (key == 'y') viewer[1] -= 1.0;
    if (key == 'Y') viewer[1] += 1.0;
    if (key == 'z') viewer[2] -= 1.0;
    if (key == 'Z') viewer[2] += 1.0;
    
    if (key == 'a' || key == 'A') {
		// slow down spaceship
		vec4 velocity = spaceship->getVelocity();

		mat4 scaleMatrix = Scale(0.9, 0.9, 0.9);
		velocity = scaleMatrix * velocity;
        if (length(velocity) < SPACESHIP_MINIMUM_SPEED) {
			velocity = normalize(velocity) * SPACESHIP_MINIMUM_SPEED;
        }

		spaceship->setVelocity(velocity);
    }
	if (key == 'd' || key == 'D') {
		// speed up spaceship
		vec4 velocity = spaceship->getVelocity();

		mat4 scaleMatrix = Scale(1.1, 1.1, 1.1);
		velocity = scaleMatrix * velocity;
		if (length(velocity) > SPACESHIP_MAXIMUM_SPEED) {
			velocity = normalize(velocity) * SPACESHIP_MAXIMUM_SPEED;
		}

		spaceship->setVelocity(velocity);
	}
    
    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        // rotate spaceship left
        vec4 rotation = spaceship->getRotation();
        mat4 rotationMatrix = RotateZ(5.0);
        //rotation = rotationMatrix * rotation;
        rotation += vec4(0, 0, 5, 0);
        spaceship->setRotation(rotation);

		// also rotate velocity
		vec4 velocity = spaceship->getVelocity();
		velocity = rotationMatrix * velocity;
		spaceship->setVelocity(velocity);
    }
	if (key == GLUT_KEY_RIGHT) {
		// rotate spaceship right
		vec4 rotation = spaceship->getRotation();
		mat4 rotationMatrix = RotateZ(-5.0);
		//rotation = rotationMatrix * rotation;
        rotation += vec4(0, 0, -5, 0);
		spaceship->setRotation(rotation);

		// also rotate velocity
		vec4 velocity = spaceship->getVelocity();
		velocity = rotationMatrix * velocity;
		spaceship->setVelocity(velocity);
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    aspect = GLfloat(width) / height;
}

//----------------------------------------------------------------------------

void update(int value) {
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->updatePhysics();
	}

	glutPostRedisplay();
	glutTimerFunc(FRAME_TIME, update, 0);
}

//----------------------------------------------------------------------------

int main(int argc, char** argv) {
    glutInit(&argc, argv);

#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

    glutInitWindowSize(512, 512);
    //    glutInitContextVersion( 3, 2 );
      //  glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow("Space Sim");
    glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutReshapeFunc(reshape);

	glutTimerFunc(FRAME_TIME, update, 0);

    glutMainLoop();
    return 0;
}
