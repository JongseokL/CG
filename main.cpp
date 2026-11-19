#include <GL/glew.h>
#include <GL/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

GLuint shaderProgramID;
GLuint VAO;
GLuint VBO;
GLint modelLocation;
GLint colorLocation;
GLint useVertexColorLocation;

float cubeVertices[] = {
	-0.5f,-0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,

	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,

	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,

	-0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f,-0.5f,

	-0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f
};

std::string filetobuf(const char* file) {
	std::ifstream shaderFile(file);

	if (!shaderFile.is_open()) return "";

	std::string source(
		(std::istreambuf_iterator<char>(shaderFile)),
		std::istreambuf_iterator<char>()
	);

	shaderFile.close();

	return source;
}

void makeShaderProgram() {
	std::string vertexSource = filetobuf("vertex.glsl");
	std::string fragmentSource = filetobuf("fragment.glsl");

	const char* vertexCode = vertexSource.c_str();
	const char* fragmentCode = fragmentSource.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);

	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgramID);
}

void InitBuffer() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color) {
	glm::mat4 modelTransform = glm::mat4(1.0f);
	modelTransform = glm::translate(modelTransform, position);
	modelTransform = glm::scale(modelTransform, size);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelTransform));
	glUniform1i(useVertexColorLocation, GL_FALSE);
	glUniform3f(colorLocation, color.r, color.g, color.b);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}\

void drawTree(float x, float z){
	const glm::vec3 trunkColor(0.38f, 0.20f,0.08f);
	const glm::vec3 leafColor1(0.10f, 0.38f, 0.10f);
	const glm::vec3 leafColor2(0.14f, 0.48f, 0.14f);

	drawCube(glm::vec3(x, 0.40f, z), glm::vec3(0.35f, 1.5f, 0.35f), trunkColor);
	drawCube(glm::vec3(x, 1.35f, z), glm::vec3(1.4f, 1.2f, 1.4f),leafColor1);
	drawCube(glm::vec3(x, 2.05f, z), glm::vec3(1.0f, 0.8f, 1.0f), leafColor2);
}

void drawParkingTrees(){
	drawTree(-5.0f, 10.5f);
	drawTree(0.0f, 10.5f);
	drawTree(5.0f, 10.5f);
	drawTree(-10.0f, -3.0f);
	drawTree(-10.0f, 3.0f);
	drawTree(-5.0f, -10.5f);
	drawTree(0.0f, -10.5f);
	drawTree(5.0f, -10.5f);
}

void drawRoadTextCube(float x, float z, float sizeX, float sizeZ){
	const glm::vec3 textColor(1.0f, 1.0f, 1.0f);
	drawCube(glm::vec3(x, -0.44f, z), glm::vec3(sizeX, 0.03f, sizeZ), textColor);
}

void drawLetterI(float x, float z){
	drawRoadTextCube(x, z, 0.9f, 0.12f);
	drawRoadTextCube(x + 0.42f, z, 0.12f, 0.55f);
	drawRoadTextCube(x - 0.42f, z, 0.12f, 0.55f);
}

void drawLetterN(float x, float z){
	drawRoadTextCube(x, z - 0.28f, 1.0f, 0.12f);
	drawRoadTextCube(x, z + 0.28f, 1.0f, 0.12f);

	for (int i = 0; i < 5; ++i){
		float blockX = x - 0.36f + i * 0.18f;
		float blockZ = z - 0.22f + i * 0.11f;
		drawRoadTextCube(blockX,blockZ, 0.16f, 0.16f);
	}
}

void drawLetterO(float x, float z){
	drawRoadTextCube(x, z - 0.28f, 1.0f, 0.12f);
	drawRoadTextCube(x, z + 0.28f, 1.0f, 0.12f);
	drawRoadTextCube(x + 0.44f, z, 0.12f, 0.55f);
	drawRoadTextCube(x - 0.44f, z, 0.12f, 0.55f);
}

void drawLetterU(float x, float z){
	drawRoadTextCube(x + 0.05f, z - 0.28f, 0.9f, 0.12f);
	drawRoadTextCube(x + 0.05f, z + 0.28f, 0.9f, 0.12f);
	drawRoadTextCube(x - 0.40f, z, 0.12f, 0.55f);
}

void drawLetterT(float x, float z){
	drawRoadTextCube(x + 0.44f, z, 0.12f, 0.60f);
	drawRoadTextCube(x - 0.03f, z, 0.94f, 0.12f);
}

void drawRoadText(){
	const float textX = 9.0f;

	drawLetterI(textX, -0.65f);
	drawLetterN(textX, -1.35f);

	drawLetterO(textX, 0.50f);
	drawLetterU(textX, 1.10f);
	drawLetterT(textX, 1.80f);
}

void drawParkingLines(){
	const glm::vec3 parkingLineColor(1.0f, 1.0f, 1.0f);

	const float lineY = -0.46f;
	const float lineHeight = 0.03f;
	const float lineThickness = 0.12f;

	const float parkingSpaceWidth = 2.0f;
	const float parkingSpaceDepth = 2.0f;

	// 뒤쪽 5칸
	for (int i = 0; i <= 5; ++i) {
		float x = -4.0f + parkingSpaceWidth * i;
		drawCube(glm::vec3(x, lineY, -6.0f), glm::vec3(lineThickness, lineHeight, parkingSpaceDepth ), parkingLineColor);
	}

	// 앞쪽 5칸
	for (int i = 0; i <= 5; ++i) {
		float x = -4.0f + parkingSpaceWidth * i;
		drawCube(glm::vec3(x, lineY, 6.0f), glm::vec3( lineThickness, lineHeight, parkingSpaceDepth ), parkingLineColor);
	}

	// 왼쪽 6칸
	for (int i = 0; i <= 5; ++i) {
		float z = -5.0f + parkingSpaceWidth * i;
		drawCube(glm::vec3(-5.0f, lineY, z), glm::vec3( parkingSpaceDepth, lineHeight, lineThickness ), parkingLineColor);
	}
}

void drawSidewalkBlock() {
	const glm::vec3 grassColor(0.22f, 0.48f, 0.18f);
	const glm::vec3 sidewalkColor1(0.62f, 0.62f, 0.58f);
	const glm::vec3 sidewalkColor2(0.55f, 0.55f, 0.52f);
	drawCube(glm::vec3(12.75f, -0.45f, -8.5f),glm::vec3(14.5f, 0.2f, 11.0f), grassColor);
	drawCube(glm::vec3(12.75f, -0.45f, 8.5f), glm::vec3(14.5f, 0.2f, 11.0f), grassColor);

	for (int i = 0; i < 14; ++i) {
		float x = 6.5f + static_cast<float>(i);

		const glm::vec3& blockColor = (i % 2 == 0) ? sidewalkColor1 : sidewalkColor2;

		drawCube(glm::vec3(x, -0.4f, -2.5f), glm::vec3(1.0f, 0.3f, 1.0f), blockColor);
		drawCube(glm::vec3(x, -0.4f, 2.5f),glm::vec3(1.0f, 0.3f, 1.0f),blockColor);
	}
}

void drawParkingBarrier(){
	const glm::vec3 barrierBodyColor(0.85f, 0.55f, 0.05f);
	const glm::vec3 barrierArmColor(0.85f, 0.10f, 0.10f);

	drawCube(glm::vec3(7.5f, 0.30f, -2.5f), glm::vec3(0.6f, 1.1f, 0.6f), barrierBodyColor);
	drawCube(glm::vec3(7.5f, 0.68f, 0.0f), glm::vec3(0.22f, 0.16f, 5.0f), barrierArmColor);
}

void drawParkingLot() {
	const glm::vec3 parkingLotColor(0.32f, 0.34f, 0.36f);
	const glm::vec3 sidewalkColor(0.62f, 0.62f, 0.58f);
	const glm::vec3 grassColor(0.22f, 0.48f, 0.18f);
	const glm::vec3 roadLineColor(1.0f, 0.8f, 0.05f);

	drawCube(glm::vec3(0.0f, -0.6f, 0.0f), glm::vec3(12.0f, 0.25f, 14.0f), parkingLotColor);
	drawCube(glm::vec3(-6.5f, -0.4f, 0.0f), glm::vec3(1.0f, 0.3f, 16.0f),sidewalkColor);
	drawCube(glm::vec3(6.5f, -0.4f, -5.0f), glm::vec3(1.0f, 0.3f, 6.0f), sidewalkColor);
	drawCube(glm::vec3(6.5f, -0.4f, 5.0f), glm::vec3(1.0f, 0.3f, 6.0f), sidewalkColor);
	drawCube(glm::vec3(13.0f, -0.6f, 0.0f), glm::vec3(14.0f, 0.25f, 4.0f), parkingLotColor);
	drawCube(glm::vec3(0.0f, -0.4f, -7.5f), glm::vec3(12.0f, 0.3f, 1.0f), sidewalkColor);
	drawCube(glm::vec3(0.0f, -0.4f, 7.5f), glm::vec3(12.0f, 0.3f, 1.0f), sidewalkColor);
	drawCube(glm::vec3(-10.0f, -0.45f, 0.0f), glm::vec3(6.0f, 0.2f, 28.0f), grassColor);
	drawCube(glm::vec3(-1.5f, -0.45f, -11.0f), glm::vec3(14.0f, 0.2f, 6.0f), grassColor);
	drawCube(glm::vec3(-1.5f, -0.45f, 11.0f), glm::vec3(14.0f, 0.2f, 6.0f), grassColor);
	drawCube(glm::vec3(13.0f, -0.46f, 0.0f), glm::vec3(14.0f, 0.03f, 0.15f), roadLineColor);

	drawParkingLines();
	drawRoadText();
	drawParkingBarrier();
	drawSidewalkBlock();
	drawParkingTrees();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	if (!glfwInit()) {
		std::cerr << "GLFW 초기화 실패!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL Window", nullptr, nullptr);

	if (!window) {
		std::cerr << "윈도우 생성 실패!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW 초기화 실패!" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	makeShaderProgram();
	InitBuffer();

	glUseProgram(shaderProgramID);

	modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	colorLocation = glGetUniformLocation(shaderProgramID, "uColor");
	useVertexColorLocation = glGetUniformLocation(shaderProgramID, "useVertexColor");

	GLint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
	GLint projectionLocation = glGetUniformLocation( shaderProgramID, "projectionTransform");

	glm::mat4 viewTransform = glm::lookAt(glm::vec3(12.0f, 10.0f, 18.0f), glm::vec3(0.0f, -0.3f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionTransform = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

	glUseProgram(shaderProgramID);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewTransform));

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionTransform));

	glViewport(0, 0, 1920, 1080);

	glfwSetKeyCallback(window, keyCallback);

	glClearColor(0.50f, 0.72f, 0.90f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgramID);

		drawParkingLot();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgramID);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
