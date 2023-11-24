#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "objects.hpp"
#include "stb_image.h"

using namespace std;
using namespace glm;





// Window dimensions
const GLint WIDTH = 1400, HEIGHT = 800;

// Callbacks
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Movement
float deltaTime = 0.0f;   // Time between this and the last frame
bool keysPressed[1024];   // Allows movement in multiple directions in one frame
void moveCam();           // Executes the camera movement

bool firstMouse = true;
float lastX; 
float lastY;

Camera cam(vec3(0.0f, 0.0f, 3.0f), STUCK_ON_WORLD_Y);
float hitRange = 10.0f;



// Daytimes
struct Daytime {
	vec3 skyColor;
	vec3 dirLightDir;
	vec3 dirLightAmbient;
	vec3 dirLightDiffuse;
	vec3 dirLightSpecular;
};

Daytime daytimes[] = {
	{vec3(0.8f, 0.9f, 1.0f), vec3(-0.7f, -0.7f, 0.0f), vec3(0.1f, 0.1f, 0.1f), vec3(1.0f, 0.9f, 0.8f), vec3(1.0f, 1.0f, 1.0f)},    // Morning Sun
	{vec3(0.5f, 0.7f, 1.0f), vec3(0.1f, -1.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f)},     // Midday Sun
	{vec3(1.0f, 0.8f, 0.7), vec3(0.7f, -0.7f, 0.0f), vec3(0.15f, 0.1f, 0.05f), vec3(0.9f, 0.8f, 0.7f), vec3(1.0f, 1.0f, 1.0f)},    // Sunset
	{vec3(0.1f, 0.1f, 0.1f), vec3(0.1f, -1.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f), vec3(0.2f, 0.2f, 0.2f), vec3(0.2f, 0.2f, 0.2f)},     // Full Moon
};

unsigned char nrDaytimes = 4;
unsigned char currentDaytime = 0;



// Spotlight
GLboolean spotlightOn = GL_FALSE;

// Gravity
void doGravity();



// Blocks and Lamps
Texture textures[13];

struct BlockType {
	short diffTexIdx;   // Index of the Texture object in the textures array
	short specTexIdx;   // Index of the Texture object in the textures array
	GLfloat shininess;
	bool gravity;
};

BlockType blockTypes[] = {
	{ 1, 0, 45.6f, true },     // Grass
	{ 4, 0, 35.6f, false },    // Dark wood
	{ 3, 8, 80.0f, false },    // Slab tiles
	{ 2, 0, 25.6f, false },    // Stone tiles
	{ 5, 0, 25.6f, false },    // Concrete
	{ 6, 0, 25.6f, false },    // Pavement
	{ 10, 0, 65.0f, true },    // Moss
	{ 11, 12, 30.6f, false }   // Metal panel
};

short nrBlockTypes = 8;

struct Block {
	vec3 position;
	BlockType type;
};

vector<Block> blocks;   // Contains all blocks set in the scene

struct LampType {
	short texIdx;   // Index of the Texture object in the textures array   

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};

LampType lampTypes[] = {
	{ 7, vec3(0.03f, 0.1f, 0.04f), vec3(0.18f, 0.49f, 0.21f), vec3(0.35f, 0.98f, 0.42f), 1.0f, 0.14f, 0.07f },   // Green paper lantern
	{ 9, vec3(0.1f, 0.1f, 0.1f), vec3(0.5f, 0.5f, 0.5f), vec3(0.7f, 0.7f, 0.7f), 1.0f, 0.14f, 0.07f }            // White paper lantern
};

short nrLampTypes = 2;

struct Lamp {
	vec3 position;
	LampType type;
};

vector<Lamp> lamps;   // Contains all lamps set in the scene



// Index of the currently selected block or lamp type.
// When the end of the blockTypes array is reached, it continues with the lampTypes array.
// That is, if the value is less than the size of blockTypes then it represents an index of blockTypes 
// and otherwise it represents an index of lampTypes obtained by subtracting the size of blockTypes from it.
short selectedBlockLampType = 0;



// Raycasting
struct Intersection {
	vec3 point;
	vec3 normal;
};

Intersection calcIntersectionRayCube(vec3 rayOrigin, vec3 rayDir, float rayRange, vec3 cubePos);
void setCube();
void destroyCube();





int main()
{
	/* -------------------------------------------------------------------------------- */
	/*                                      SET UP                                      */
	/* -------------------------------------------------------------------------------- */

	// Initialize GLFW
	if (! glfwInit())
	{
		std::cerr << "ERROR::GLFW::INITIALIZATION_FAILED" << endl;
		return -1;
	}

	// Set OpenGL context attributes
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft Clone", NULL, NULL);
	if (! window)
	{
		std::cerr << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (! gladLoadGL())
	{
		std::cerr << "ERROR::GLAD::INITIALIZATION_FAILED" << endl;
		glfwTerminate();
		return -1;
	}

	// OpenGL options
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST); 

	// Register callbacks
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Mouse is captured within the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  



	// Shaders
	const Shader blockShader("./lighting.vert", "./lighting.frag");
	const Shader lampShader("./lamp.vert", "./lamp.frag");
	const Shader crosshairShader("./crosshair.vert", "./crosshair.frag");
	
	// Textures
	textures[0] = Texture("./noSpecular.png");
	textures[1] = Texture("./grassDiffuse.png");
	textures[2] = Texture("./stone_tiles_diff.jpg");
	textures[3] = Texture("./slab_tiles_diff.jpg");
	textures[4] = Texture("./dark_wood_diff.jpg");
	textures[5] = Texture("./concrete_wall_diff.jpg");
	textures[6] = Texture("./pavement_diff.jpg");
	textures[7] = Texture("./green_paper_lantern.jpg"); 
	textures[8] = Texture("./slab_tiles_spec.jpg");
	textures[9] = Texture("./white_paper_lantern.jpg"); 
	textures[10] = Texture("./moss_diff.jpg");
	textures[11] = Texture("./metal_panel_diff.jpg");
	textures[12] = Texture("./metal_panel_diff.jpg");
	
	// Generate blocks for the launch platform
	for (float i = -10.0f; i <= 10.0f; i++)        
	{
		for (float j = -10.0f; j <= 10.0f; j++)   
		{
			blocks.push_back({ vec3(i, 0.0f, j), blockTypes[6] });
		}
	}

	cam.pos = vec3(0.0f, 2.0f, 0.0f);

	// Set constant uniforms
	blockShader.use();
	blockShader.setUniform("spotLight.innerCutOff", cos(radians(5.0f)));
	blockShader.setUniform("spotLight.outerCutOff", cos(radians(13.5f)));
	blockShader.setUniform("spotLight.ambient", vec3(0.2f, 0.2f, 0.2f));
	blockShader.setUniform("spotLight.diffuse", vec3(0.9f, 0.9f, 0.9f));
	blockShader.setUniform("spotLight.specular", vec3(1.0f, 1.0f, 1.0f));
	blockShader.setUniform("spotLight.constant", 1.0f);
	blockShader.setUniform("spotLight.linear", 0.09f);
	blockShader.setUniform("spotLight.quadratic", 0.032f);
	


	
	
	/* -------------------------------------------------------------------------------- */
	/*                                   RENDER LOOP                                    */
	/* -------------------------------------------------------------------------------- */

	glm::vec3 clearColor;
	glm::mat4 transform, projection, view, model;   // transformation matrices

	float currentFrame;       // Point in time of the current frame
	float lastFrame = 0.0f;   // Point in time of the last frame

	while (! glfwWindowShouldClose(window))
	{
		// Clear screen
		clearColor = daytimes[currentDaytime].skyColor;
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		projection = glm::perspective(radians(cam.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 150.0f);
		view = cam.getViewMatrix();
	
		/* -------------------------------------------------------------------------------- */
		/*                                    DRAW BLOCKS                                   */
		/* -------------------------------------------------------------------------------- */

		blockShader.use();

		// Directional light
		blockShader.setUniform("dirLight.direction", daytimes[currentDaytime].dirLightDir);
		blockShader.setUniform("dirLight.ambient", daytimes[currentDaytime].dirLightAmbient);
		blockShader.setUniform("dirLight.diffuse", daytimes[currentDaytime].dirLightDiffuse);
		blockShader.setUniform("dirLight.specular", daytimes[currentDaytime].dirLightSpecular);

		// Point lights
		int nrLamps = static_cast<int>(lamps.size());
		blockShader.setUniform("nrPointLights", nrLamps);
		for (int i = 0; i < nrLamps; i++)
		{
			blockShader.setUniform("pointLights[" + to_string(i) + "].position", lamps[i].position); 
			blockShader.setUniform("pointLights[" + to_string(i) + "].ambient", lamps[i].type.ambient);
			blockShader.setUniform("pointLights[" + to_string(i) + "].diffuse", lamps[i].type.diffuse);
			blockShader.setUniform("pointLights[" + to_string(i) + "].specular", lamps[i].type.specular);
			blockShader.setUniform("pointLights[" + to_string(i) + "].constant", lamps[i].type.constant);
			blockShader.setUniform("pointLights[" + to_string(i) + "].linear", lamps[i].type.linear);
			blockShader.setUniform("pointLights[" + to_string(i) + "].quadratic", lamps[i].type.quadratic);
		}
		
		// Spotlight
		blockShader.setUniform("spotLightOn", spotlightOn);
		blockShader.setUniform("spotLight.position", cam.pos);
		blockShader.setUniform("spotLight.direction", cam.front);

		// Camera position
		blockShader.setUniform("camPos", cam.pos);

		// Draw blocks
		for (const Block& block : blocks) 
		{
			// Translate
			model = glm::mat4(1.0f);
			model = glm::translate(model, block.position);

			// Transformation matrices
			transform = projection * view * model; 
			blockShader.setUniform("modelMat", model);
			blockShader.setUniform("transformMat", transform); 

			// Material
			textures[block.type.diffTexIdx].bindToTexUnit(GL_TEXTURE0);
			textures[block.type.specTexIdx].bindToTexUnit(GL_TEXTURE1);
			blockShader.setUniform("material.diffuseTexture", 0);
			blockShader.setUniform("material.specularTexture", 1);
			blockShader.setUniform("material.shininess", block.type.shininess);

			drawBlock(); 
		}

		/* -------------------------------------------------------------------------------- */
		/*                                    DRAW LAMPS                                    */
		/* -------------------------------------------------------------------------------- */

		lampShader.use();

		// Draw lamps
		for (const Lamp& lamp : lamps)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, lamp.position);

			transform = projection * view * model; 
			lampShader.setUniform("transformMat", transform);

			textures[lamp.type.texIdx].bindToTexUnit(GL_TEXTURE0);
			lampShader.setUniform("lampTexture", 0);

			drawLamp();
		}



		// Draw crosshair
		glDisable(GL_DEPTH_TEST);
		crosshairShader.use();
		float crosshairWidth = 1.0f;
		float crosshairHeight = crosshairWidth * WIDTH / HEIGHT;
		transform = glm::scale(mat4(1.0f), vec3(crosshairWidth, crosshairHeight, 1.0f));
		crosshairShader.setUniform("transformMat", transform);
		drawCrosshair();
		glEnable(GL_DEPTH_TEST);



		// Display frame
		glfwSwapBuffers(window);

		// Calculate delta time
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Gravity
		doGravity();

		// Respond to user input
		glfwPollEvents();
		moveCam();
	}



	/* -------------------------------------------------------------------------------- */
	/*                                     CLEAN UP                                     */
	/* -------------------------------------------------------------------------------- */
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}





void error_callback(int error, const char *description)
{
	std::cerr << description << endl;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_LEFT_CONTROL) 
	{
		if (action == GLFW_PRESS) 
			cam.movementSpeed *= 2;
		else if (action == GLFW_RELEASE) 
			cam.movementSpeed /= 2;
	}
	else if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		spotlightOn = ~spotlightOn;
	}
	else if (key == GLFW_KEY_C)
	{
		if (action == GLFW_PRESS)
			cam.fov = 8.0f;
		if (action == GLFW_RELEASE)
			cam.fov = 45.0f;
	}
	else if (key == GLFW_KEY_T && action == GLFW_RELEASE)
	{
		currentDaytime++;
		if (currentDaytime == nrDaytimes)
			currentDaytime = 0;
	}
	else if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
			keysPressed[key] = true;
		else if (action == GLFW_RELEASE)
			keysPressed[key] = false;
	}
}


void moveCam()
{
	if (keysPressed[GLFW_KEY_W])
	{
		cam.move(FORWARD, deltaTime);
	}
	if (keysPressed[GLFW_KEY_S])
	{
		cam.move(BACKWARD, deltaTime);
	}
	if (keysPressed[GLFW_KEY_A])
	{
		cam.move(LEFT, deltaTime);
	}
	if (keysPressed[GLFW_KEY_D])
	{
		cam.move(RIGHT, deltaTime);
	}
	if (keysPressed[GLFW_KEY_SPACE])
	{
		cam.move(UP, deltaTime);
	}
	if (keysPressed[GLFW_KEY_LEFT_SHIFT])
	{
		cam.move(DOWN, deltaTime);
	}
}


void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	cam.lookAround(xoffset, yoffset);
}


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	if (yoffset < 0)
	{
		if (selectedBlockLampType == (nrBlockTypes + nrLampTypes - 1))
			selectedBlockLampType = 0;
		else
			selectedBlockLampType++;
	}
	else if (yoffset > 0)
	{
		if (selectedBlockLampType == 0)
			selectedBlockLampType = nrBlockTypes + nrLampTypes - 1;
		else
			selectedBlockLampType--;
	}
}


const float clickInterval = 0.1f;
float lastClickTime = 0.0f;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	float currentTime = glfwGetTime();

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (currentTime - lastClickTime >= clickInterval)
		{
			setCube();
			lastClickTime = currentTime;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (currentTime - lastClickTime >= clickInterval)
		{
			destroyCube();
			lastClickTime = currentTime;
		}
	}
}


void setCube()
{
	glm::vec3 newCubePos(std::numeric_limits<float>::infinity());
	glm::vec3 nearestIntersectionPoint(std::numeric_limits<float>::infinity());
	Intersection intersection;

	// Calculate new cube position
	for (const Block& block : blocks)
	{
		// Skip blocks that are too far away (for the performance)
		if (glm::length(cam.pos - block.position) > hitRange)
			continue;

		intersection = calcIntersectionRayCube(cam.pos, cam.front, hitRange, block.position);
		if (glm::length(cam.pos - intersection.point) < glm::length(cam.pos - nearestIntersectionPoint))
		{
			nearestIntersectionPoint = intersection.point;
			newCubePos = block.position + intersection.normal;
		}
	}
	for (const Lamp& lamp : lamps)
	{
		// Skip lamps that are too far away (for the performance)
		if (glm::length(cam.pos - lamp.position) > hitRange)
			continue;

		intersection = calcIntersectionRayCube(cam.pos, cam.front, hitRange, lamp.position);
		if (glm::length(cam.pos - intersection.point) < glm::length(cam.pos - nearestIntersectionPoint))
		{
			nearestIntersectionPoint = intersection.point;
			newCubePos = lamp.position + intersection.normal;
		}
	}

	// Return if no new cube position could be calculated
	if (newCubePos.x == std::numeric_limits<float>::infinity())
		return;

	for (const Block& block : blocks)
	{
		if (block.position == newCubePos)
			return;
	}
	for (const Lamp& lamp : lamps)
	{
		if (lamp.position == newCubePos)
			return;
	}
		
	// Set new block or lamp
	if (selectedBlockLampType < nrBlockTypes)
		blocks.push_back({ newCubePos, blockTypes[selectedBlockLampType] });
	else
		lamps.push_back({ newCubePos, lampTypes[selectedBlockLampType - nrBlockTypes] });
}

void destroyCube()
{
	int hitBlockIdx = -1;
	int hitLampIdx = -1;
	glm::vec3 nearestIntersectionPoint(std::numeric_limits<float>::infinity());
	Intersection intersection;

	// Calculate which cube was hit
	for (int i = 0; i < blocks.size(); i++)
	{
		// Skip blocks that are too far away (for the performance)
		if (glm::length(cam.pos - blocks[i].position) > hitRange)
			continue;

		intersection = calcIntersectionRayCube(cam.pos, cam.front, hitRange, blocks[i].position);
		if (glm::length(cam.pos - intersection.point) < glm::length(cam.pos - nearestIntersectionPoint))
		{
			nearestIntersectionPoint = intersection.point;
			hitBlockIdx = i;
			hitLampIdx = -1;
		}
	}
	for (int i = 0; i < lamps.size(); i++)
	{
		// Skip lamps that are too far away (for the performance)
		if (glm::length(cam.pos - lamps[i].position) > hitRange)
			continue;

		intersection = calcIntersectionRayCube(cam.pos, cam.front, hitRange, lamps[i].position);
		if (glm::length(cam.pos - intersection.point) < glm::length(cam.pos - nearestIntersectionPoint))
		{
			nearestIntersectionPoint = intersection.point;
			hitLampIdx = i;
			hitBlockIdx = -1;
		}
	}

	if (hitBlockIdx >= 0)
		blocks.erase(blocks.begin() + hitBlockIdx);
	if (hitLampIdx >= 0)
		lamps.erase(lamps.begin() + hitLampIdx);
}


Intersection calcIntersectionRayCube(vec3 rayOrigin, vec3 rayDir, float rayRange, vec3 cubePos)
{
	glm::vec3 cubeMin = cubePos - vec3(0.5f);
	glm::vec3 cubeMax = cubePos + vec3(0.5f);

	glm::vec3 tMin = (cubeMin - rayOrigin) / rayDir;
	glm::vec3 tMax = (cubeMax - rayOrigin) / rayDir;

	glm::vec3 tEntry = glm::min(tMin, tMax);
	glm::vec3 tExit = glm::max(tMin, tMax);

	float tNear = glm::max(glm::max(tEntry.x, tEntry.y), tEntry.z);
	float tFar = glm::min(glm::min(tExit.x, tExit.y), tExit.z); 

	if (tNear >= 0.0f && tNear <= rayRange && tNear < tFar)
	{
		glm::vec3 intersectionPoint = rayOrigin + tNear * rayDir;

		glm::vec3 normal(0.0f);
		if (intersectionPoint.x == cubePos.x + 0.5f)
			normal = vec3(1.0f, 0.0f, 0.0f);
		else if (intersectionPoint.y == cubePos.y + 0.5f)
			normal = vec3(0.0f, 1.0f, 0.0f);
		else if (intersectionPoint.z == cubePos.z + 0.5f)
			normal = vec3(0.0f, 0.0f, 1.0f);
		else if (intersectionPoint.x == cubePos.x - 0.5f)
			normal = vec3(-1.0f, 0.0f, 0.0f);
		else if (intersectionPoint.y == cubePos.y - 0.5f)
			normal = vec3(0.0f, -1.0f, 0.0f);
		else if (intersectionPoint.z == cubePos.z - 0.5f)
			normal = vec3(0.0f, 0.0f, -1.0f);
		

		return { intersectionPoint, normal };
	}

	return { glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(0.0f) };
}


float gravity = 4.25f;   

void doGravity()
{
	for (Block& block : blocks)
	{
		if (! block.type.gravity || block.position.y == 0)
			continue;

		int targetY = 0;

		for (const Block& otherBlock : blocks)
		{
			if (block.position.x == otherBlock.position.x &&
				block.position.z == otherBlock.position.z &&
				otherBlock.position.y < block.position.y &&
				otherBlock.position.y >= targetY)
			{
				targetY = otherBlock.position.y + 1;
			}
		}
		for (const Lamp& otherLamp : lamps)
		{
			if (block.position.x == otherLamp.position.x &&
				block.position.z == otherLamp.position.z &&
				otherLamp.position.y < block.position.y &&
				otherLamp.position.y >= targetY)
			{
				targetY = otherLamp.position.y + 1;
			}
		}

		if (block.position.y > targetY)
		{
			block.position.y -= gravity * deltaTime;

			// Check that the block didn't fall below targetY
			if (block.position.y < targetY)
				block.position.y = targetY;
		}
	}
}