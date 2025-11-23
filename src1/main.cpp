#include <iostream>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
static void MouseCallback(GLFWwindow *window, double xpos, double ypos);
//static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
int windowWidth = 800;
int windowHeight = 600;
Camera *camera;
int main(int argc, char **argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPEN_FOREWARD_COMPAT, GL_TRUE);
	#endif
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if(window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//glfwSetKeyCallback(window, KeyboardCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	GLfloat vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
	GLuint VBO, VAO,lightVAO, texture, textureSpecular;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shader = Shader("shaders/vertex.vs", "shaders/fragment1.fs");
	Shader lightShader = Shader("shaders/vertex.vs", "shaders/lightfragment.fs");
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("images/container2.png", &width, &height, &nrChannels, 0);
	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glGenTextures(1, &textureSpecular);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("images/container2_specular.png", &width, &height, &nrChannels, 0);
	format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glm::mat4 model;	
	glm::vec3 objectPos[] = {glm::vec3(1.0f, 1.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
					glm::vec3(2.0f, 1.0f, 2.0f),
					glm::vec3(0.5f, -1.4f, -2.0f),
					glm::vec3(-20.0f, -2.6f, -1.0f)};
	glm::vec3 objectAmbient = glm::vec3(0.24725, 0.1995, 0.0745);
	glm::vec3 objectDiffuse = glm::vec3(0.75164, 0.60648, 0.22648);
	glm::vec3 objectSpecular = glm::vec3(0.628281, 0.555802, 0.366065);
	GLfloat objectShiny = 51.2f;
	glm::vec3 lightPos[] = {glm::vec3(1.0f, 1.5f, -1.4f), glm::vec3(0.8f, -1.5f, -3.4f),};
	glm::vec3 lightAmbient = glm::vec3(0.2f);
	glm::vec3 lightDiffuse = glm::vec3(1.0f);
	glm::vec3 lightSpecular = glm::vec3(1.0f);

	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(55.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		processInput(window);
		projection = glm::perspective(glm::radians(55.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		camera->setViewMatrix();

				
		shader.use();
		shader.setMatrix4("view", camera->getViewMatrix());
		shader.setMatrix4("projection", projection);
		//shader.setVector3("material.ambient", objectAmbient);
		shader.setInt("material.diffuse", 0);
		shader.setInt("material.specular", 1);
		shader.setFloat("material.shininess", objectShiny);
		shader.setVector3("dirLight.ambient", lightAmbient);
		shader.setVector3("dirLight.diffuse", lightDiffuse);
		shader.setVector3("dirLight.specular", lightSpecular);
		shader.setVector3("dirLight.direction", glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setFloat("light.cutOff", cos(12.5f));
		shader.setFloat("light.outerCutOff", cos(17.5f));
		shader.setVector3("pointLights[0].position", lightPos[0]);
		shader.setVector3("pointLights[0].ambient", lightAmbient);
		shader.setVector3("pointLights[0].diffuse", lightDiffuse);
		shader.setVector3("pointLights[0].specular", lightSpecular);
		shader.setFloat("pointLights[0].constant", 1.0f);
		shader.setFloat("pointLights[0].linear", 0.09f);
		shader.setFloat("pointLights[0].quadratic[0]", 0.032f);

		shader.setVector3("pointLights[1].position", lightPos[1]);
		shader.setVector3("pointLights[1].ambient", lightAmbient);
		shader.setVector3("pointLights[1].diffuse", lightDiffuse);
		shader.setVector3("pointLights[1].specular", lightSpecular);
		shader.setFloat("pointLights[1].constant", 1.0f);
		shader.setFloat("pointLights[1].linear", 0.09f);
		shader.setFloat("pointLights[1].quadratic[0]", 0.032f);

		shader.setVector3("spotLight.position", camera->position);
		shader.setVector3("spotLight.direction", camera->direction);
		shader.setFloat("spotLight.cutOff", cos(5.5f));
		shader.setFloat("spotLight.outerCutOff", cos(7.5f));
		shader.setVector3("spotLight.ambient", lightAmbient);
		shader.setVector3("spotLight.diffuse", lightDiffuse);
		shader.setVector3("spotLight.specular", lightSpecular);
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09f);
		shader.setFloat("spotLight.quadratic", 0.032f);


		shader.setVector3("cameraPos", camera->position);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureSpecular);
		glBindVertexArray(VAO);
		for(int i = 0; i < 5; i++){
			model = glm::mat4(1.0f);
			model = glm::translate(model, objectPos[i]);
			//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			shader.setMatrix4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for(int i = 0 ; i<2;i++){

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos[i]);
		model = glm::scale(model, glm::vec3(0.2f));

		glBindVertexArray(lightVAO);
		lightShader.use();
		lightShader.setMatrix4("model", model);
		lightShader.setMatrix4("view", camera->getViewMatrix());
		lightShader.setMatrix4("projection", projection);
		lightShader.setVector3("color", lightDiffuse);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
	}
	delete camera;
	glfwTerminate();
	
	return 0;
	
}

static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0,0,width,height);
	windowWidth = width;
	windowHeight = height;
}


static void MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	camera->setDirection((float)xpos, (float)ypos);	
}

//static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	camera->movePosition(CAMERA_MOVE_FOREWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	camera->movePosition(CAMERA_MOVE_BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	camera->movePosition(CAMERA_MOVE_LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	camera->movePosition(CAMERA_MOVE_RIGHT);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	camera->movePosition(CAMERA_MOVE_UP);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	camera->movePosition(CAMERA_MOVE_DOWN);
}
