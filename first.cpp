#include "first.h"
#include "Angel.h"
#include "gameobjects.h"
#include "planet.h"
#include "spaceship.h"
#include "ground.h"
#include "spacestation.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

// Viewing transformation parameters
GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

int currentCameraMode = CAMERA_MODE_FIRST_PERSON;

int isGameRunning = 1;

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

SpaceShip* spaceship;
SpaceStation* spaceStation;

// OpenGL initialization
void init()
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
    glUseProgram(program);
    viewProjection = glGetUniformLocation(program, "ViewProjection");

	GLuint ambientProduct = glGetUniformLocation(program, "AmbientProduct");
	GLuint diffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
	GLuint specularProduct = glGetUniformLocation(program, "SpecularProduct");
	GLuint lightPosition0 = glGetUniformLocation(program, "LightPosition[0]");
	GLuint lightPosition1 = glGetUniformLocation(program, "LightPosition[1]");
	GLuint lightPosition2 = glGetUniformLocation(program, "LightPosition[2]");
	GLuint lightPosition3 = glGetUniformLocation(program, "LightPosition[3]");
	GLuint numActiveLights = glGetUniformLocation(program, "NumActiveLights");
	GLuint shininess = glGetUniformLocation(program, "Shininess");

	// set up light properties
	glUniform4fv(ambientProduct, 1, vec4(0.02, 0.02, 0.02, 1.0));
	glUniform4fv(diffuseProduct, 1, vec4(1.0, 1.0, 1.0, 1.0));
	glUniform4fv(specularProduct, 1, vec4(1.0, 1.0, 1.0, 1.0));
	glUniform4fv(lightPosition0, 1, vec4(0.0, 0.0, 1.0, 0.0));
	glUniform4fv(lightPosition1, 1, vec4(200.0, 200.0, 1.0, 0.0));
	glUniform4fv(lightPosition2, 1, vec4(167.0, -50.0, 100.0, 0.0));
	//glUniform4fv(lightPosition3, 1, vec4(0.0, 0.0, 1.0, 0.0));
	glUniform1i(numActiveLights, 3); // can go up to 4
	glUniform1f(shininess, 100.0);

	// draw ground
	gameObjects.push_back(new Ground(vec3(0, 0, 0), 5, 500, 500, vec4(1.0, 1.0, 1.0, 1.0), program));

	// create spaceship
	spaceship = new SpaceShip(vec3(105, 0, 15), vec3(0, 0, 0), 1.0f, 0.2f, vec4(1, 0, 0, 1), 50, program);
	spaceship->setVelocity(vec4(0.0, -1.0, 0.0, 0.0));
	gameObjects.push_back(spaceship);

	// create spacestation
	spaceStation = new SpaceStation(vec3(100, 10, 10), 7.5f, vec4(0.2, 0.2, 0.2, 1.0), 50, program);
	gameObjects.push_back(spaceStation);

	// create planets
    int planetResolution = 40;
    float planetRadius = 5.0f;
    gameObjects.push_back(new Planet(vec3(30, 170, 15), vec3(0, 0, 0), planetRadius, vec4(1.00, 0.00, 0.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(80, 110, 25), vec3(0, 0, 0), planetRadius, vec4(0.00, 1.00, 0.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(70, 60, 12), vec3(0, 0, 0), planetRadius, vec4(0.00, 0.00, 1.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(90, 150, 13), vec3(0, 0, 0), planetRadius, vec4(1.00, 1.00, 0.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(120, 80, 17), vec3(0, 0, 0), planetRadius, vec4(1.00, 0.00, 1.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(150, 40, 15), vec3(0, 0, 0), planetRadius, vec4(0.00, 1.00, 1.00, 1.0), planetResolution, program));
    gameObjects.push_back(new Planet(vec3(160, 170, 22), vec3(0, 0, 0), planetRadius, vec4(1.00, 1.00, 1.00, 1.0), planetResolution, program));

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void setCameraMatrix() {
	if (currentCameraMode == CAMERA_MODE_FIRST_PERSON) {
		// get spaceship position and set camera in front of it
		vec4 spaceshipPosition = spaceship->getPosition();
		vec4 spaceshipVelocity = spaceship->getVelocity();
		vec4 position = spaceshipPosition + normalize(spaceshipVelocity) * spaceship->getRadius()*2;
		float leverage = 1;
		position.z += leverage;
		viewer[0] = position.x;
		viewer[1] = position.y;
		viewer[2] = position.z;

		point4  eye(viewer[0], viewer[1], viewer[2], 1.0);
		// look at the direction of spaceship
		point4 at(position + (normalize(spaceshipVelocity)));
		vec4    up(0.0, 0.0, 1.0, 0.0);
		mat4  viewMatrix = LookAt(eye, at, up);
		mat4  projectionMatrix = Perspective(fovy, aspect, zNear, zFar);
		mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		glUniformMatrix4fv(viewProjection, 1, GL_TRUE, viewProjectionMatrix);
	}
	else if (currentCameraMode == CAMERA_MODE_THIRD_PERSON) {
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
	}
	else if (currentCameraMode == CAMERA_MODE_TOP_DOWN) {
		// move camera to 0,0,150
		viewer[0] = 0;
		viewer[1] = 0;
		viewer[2] = 150;

		point4  eye(viewer[0], viewer[1], viewer[2], 1.0);
		// look at down
		point4 at(0, 0, 0);
		vec4    up(0.0, 1.0, 0.0, 0.0);
		mat4  viewMatrix = LookAt(eye, at, up);
		mat4  projectionMatrix = Perspective(fovy, aspect, zNear, zFar);
		mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		glUniformMatrix4fv(viewProjection, 1, GL_TRUE, viewProjectionMatrix);
	}
	else if (currentCameraMode == CAMERA_MODE_SPACE_STATION) {
		// set camera in front of space station
		vec4 spaceStationPosition = spaceStation->getPosition();
		vec4 spaceStationDirection = spaceStation->getDirection();
		vec4 position = spaceStationPosition + normalize(spaceStationDirection) * spaceStation->getRadius() * 2;
		float leverage = 5;
		position.z += leverage;
		viewer[0] = position.x;
		viewer[1] = position.y;
		viewer[2] = position.z;

		point4  eye(viewer[0], viewer[1], viewer[2], 1.0);
		// look at the direction of space station
		point4 at(position + (normalize(spaceStationDirection)));
		vec4    up(0.0, 0.0, 1.0, 0.0);
		mat4  viewMatrix = LookAt(eye, at, up);
		mat4  projectionMatrix = Perspective(fovy, aspect, zNear, zFar);
		mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		glUniformMatrix4fv(viewProjection, 1, GL_TRUE, viewProjectionMatrix);
	}
}

//----------------------------------------------------------------------------

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCameraMatrix();

	// update every game object's buffer
    for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->updateBuffers();
    }

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void updateHelper() {
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->updatePhysics();
	}
}

void printDebug() {
	printf("Spaceship position: %f %f %f\n", spaceship->getPosition().x, spaceship->getPosition().y, spaceship->getPosition().z);
	printf("Spaceship velocity: %f %f %f\n", spaceship->getVelocity().x, spaceship->getVelocity().y, spaceship->getVelocity().z);
	printf("Spaceship rotation: %f %f %f\n", spaceship->getRotation().x, spaceship->getRotation().y, spaceship->getRotation().z);
	printf("Spacestation position: %f %f %f\n", spaceStation->getPosition().x, spaceStation->getPosition().y, spaceStation->getPosition().z);
	printf("Spacestation rotation: %f %f %f\n", spaceStation->getRotation().x, spaceStation->getRotation().y, spaceStation->getRotation().z);
	printf("Spacestation rotational velocity: %f %f %f\n", spaceStation->getRotationalVelocity().x, spaceStation->getRotationalVelocity().y, spaceStation->getRotationalVelocity().z);

	printf("\n");
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A') {
		if (currentCameraMode != CAMERA_MODE_SPACE_STATION) {
			// speed up spaceship
			vec4 velocity = spaceship->getVelocity();

			mat4 scaleMatrix = Scale(1.1, 1.1, 1.1);
			velocity = scaleMatrix * velocity;
			if (length(velocity) > SPACESHIP_MAXIMUM_SPEED) {
				velocity = normalize(velocity) * SPACESHIP_MAXIMUM_SPEED;
			}

			spaceship->setVelocity(velocity);
		}
    }
	if (key == 'd' || key == 'D') {
		if (currentCameraMode != CAMERA_MODE_SPACE_STATION) {
			// slow down spaceship
			vec4 velocity = spaceship->getVelocity();

			mat4 scaleMatrix = Scale(0.9, 0.9, 0.9);
			velocity = scaleMatrix * velocity;
			if (length(velocity) < SPACESHIP_MINIMUM_SPEED) {
				velocity = normalize(velocity) * SPACESHIP_MINIMUM_SPEED;
			}

			spaceship->setVelocity(velocity);
		}
	}

	if (key == 'j' || key == 'J') {
		// increase rotational velocity of spacestation
		vec4 rotationalVelocity = spaceStation->getRotationalVelocity();
		rotationalVelocity += vec4(0, 0, ROTATIONAL_VELOCITY_STEP, 0);
		spaceStation->setRotationalVelocity(rotationalVelocity);
	}
	if (key == 'k' || key == 'K') {
		// decrease rotational velocity of spacestation
		vec4 rotationalVelocity = spaceStation->getRotationalVelocity();
		rotationalVelocity -= vec4(0, 0, ROTATIONAL_VELOCITY_STEP, 0);
		if (rotationalVelocity.z < 0) rotationalVelocity = vec4(0, 0, 0, 0);
		spaceStation->setRotationalVelocity(rotationalVelocity);
	}

	if (key == 'c' || key == 'C') {
		currentCameraMode = CAMERA_MODE_FIRST_PERSON;
	}
	if (key == 's' || key == 'S') {
		currentCameraMode = CAMERA_MODE_SPACE_STATION;
	}
	if (key == 't' || key == 'T') {
		currentCameraMode = CAMERA_MODE_THIRD_PERSON;
	}
	if (key == 'w' || key == 'W') {
		currentCameraMode = CAMERA_MODE_TOP_DOWN;
	}
    
	if (key == 'p' || key == 'P') {
		if (isGameRunning) {
			isGameRunning = 0;
		}
		else {
			isGameRunning = 1;
		}
	}

	if (key == 'o' || key == 'O') {
		if (isGameRunning) {
			isGameRunning = 0;
		}
		else {
			// advance one frame
			updateHelper();

			glutPostRedisplay();

			printDebug();
		}
	}

    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
		if (currentCameraMode != CAMERA_MODE_SPACE_STATION) {
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
    }
	if (key == GLUT_KEY_RIGHT) {
		if (currentCameraMode != CAMERA_MODE_SPACE_STATION) {
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
	if (isGameRunning) {
		updateHelper();
		glutPostRedisplay();
	}

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
