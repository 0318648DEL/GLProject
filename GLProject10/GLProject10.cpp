#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm.hpp>
#include <gl/ext.hpp>
#include <gl/gtc/matrix_transform.hpp>

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
GLuint CompileShaders(const char* filenameVS, const char* filenameFS);
bool ReadFile(const char* filename, std::string* target);

glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 model_trans = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projM = glm::mat4(1.0f);


GLuint pShaderProgram;
GLvoid drawScene();
void Keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
GLvoid Timer(int value);

std::string buf = "000000";
int sec, min, hour;
float rad_y = 0;
float moved_z = 0;

GLfloat c_pos[]
{
	-0.05f,-0.2f,0.2f,
	0.05f,-0.2f,0.2f,
	0.05f,0.2f,0.2f,
	-0.05f,0.2f,0.2f,
	-0.05f,-0.2f,-0.2f,
	0.05f,-0.2f,-0.2f,
	0.05f,0.2f,-0.2f,
	-0.05f,0.2f,-0.2f
};

unsigned int c_index[]
{
	0,1,3,
	2,3,1,
	1,5,2,
	6,2,5,
	5,4,6,
	7,6,4,
	4,0,7,
	3,7,0,
	3,2,7,
	6,7,2,
	4,5,0,
	1,0,5
};

GLuint vao, vbo, ebo;

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example09");


	//glew초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW.\n";
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW initialized.\n";

	pShaderProgram = glCreateProgram();
	pShaderProgram = CompileShaders("vs.glsl", "fs.glsl");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(c_pos), c_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(c_index), c_index, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);



	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(10, Timer, 1);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

}

GLvoid drawScene()
{
	glUseProgram(pShaderProgram);

	glEnable(GL_CULL_FACE);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int modelLocation = glGetUniformLocation(pShaderProgram, "modelTransform");
	unsigned int viewLocation = glGetUniformLocation(pShaderProgram, "viewTransform");
	unsigned int projLocation = glGetUniformLocation(pShaderProgram, "projTransform");
	glm::mat4 world = glm::rotate(glm::mat4(1.f), glm::radians(-30.f), glm::vec3(1.f, 0.f, 0.f));
	world = glm::rotate(world, glm::radians(-30.f), glm::vec3(0.f, 1.f, 0.f));
	world = glm::rotate(world, glm::radians(rad_y), glm::vec3(0.f, 1.f, 0.f));
	world = glm::translate(world, glm::vec3(0.f, 0.f, moved_z));
	for (int num = 0; num < 6; ++num)
	{

		switch (buf[num])
		{
		case '0':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, -0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '1':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '2':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.0f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, -0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '3':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.0f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, -0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '4':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.0f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '5':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.0f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, -0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '6':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.0f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, -0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '7':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '8':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.0f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, -0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		case '9':
			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.0f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, 0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.1f, -0.6f, 0.0f));
			model_trans = glm::rotate(model_trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 2.3f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, 0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			glBindVertexArray(vao);
			model_trans = glm::mat4(1.0f);
			model_trans = glm::translate(model_trans, glm::vec3((0.7f * num) - 1.9f, -0.3f, 0.f));
			model_trans = world * model_trans;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_trans));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		}
	}


	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	projM = glm::mat4(1.0f);
	projM = glm::perspective(glm::radians(60.0f), 800.f / 600.f, 0.1f, 100.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projM));

	//그리기 부분 구현


	//그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'y':
		rad_y -= 3.5f;
		break;
	case 'Y':
		rad_y += 3.5f;
		break;
	case 'z':
		moved_z -= 0.2f;
		break;
	case 'Z':
		moved_z += 0.2f;
		break;
	}
}

GLvoid Timer(int value)
{
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(p);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%H%M%S");
	buf = ss.str();
	glutTimerFunc(10, Timer, 1);
	glutPostRedisplay();
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}


bool ReadFile(const char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint CompileShaders(const char* filenameVS, const char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}