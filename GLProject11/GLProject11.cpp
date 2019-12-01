#include <iostream>
#include <cmath>
#include <fstream>
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

GLuint pShaderProgram;
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);

GLuint p_vao, t_vao;
GLuint p_vbo, t_vbo, p_ebo;

float tri_opened = 30.f;
float front_opened = 0.f;
float up_turning = 90.f;
float left_turning = 0.f;
bool open_flag = false;
bool front_flag = false;
bool up_flag = false;
bool draw_mode = true;

glm::mat4 model_transform = glm::mat4(1.f);
glm::mat4 front_plane = glm::mat4(1.f);
glm::mat4 up_plane = glm::mat4(1.f);
glm::mat4 open_plane = glm::mat4(1.f);
glm::mat4 world_transform = glm::mat4(1.f);
glm::mat4 proj_transform = glm::mat4(1.f);

glm::vec4 color;

GLfloat plane[]
{
	-0.4f,0.f,0.f,
	0.4f,0.f,0.f,
	0.4f,0.8f,0.f,
	-0.4f,0.8f,0.f,
};

GLfloat tri[]
{
	-0.4f,0.f,0.f,
	0.4f,0.f,0.f,
	0.f,0.8f,0.f
};

unsigned int p_index[]
{
	0,1,3,
	2,3,1
};

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example11");


	//glew�ʱ�ȭ�ϱ�
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

	proj_transform = glm::ortho(-2.f, 2.f, -2.f, 2.f, 0.1f, 200.f);
	proj_transform = glm::translate(proj_transform, glm::vec3(0.f, 0.f, -3.f));

	glGenVertexArrays(1, &p_vao);
	glGenVertexArrays(1, &t_vao);
	glGenBuffers(1, &p_vbo);
	glGenBuffers(1, &t_vbo);
	glGenBuffers(1, &p_ebo);

	glBindVertexArray(p_vao);

	glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p_index), p_index, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(t_vao);

	glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timer, 0);
	glutMainLoop();
}

GLvoid drawScene()
{
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pShaderProgram);

	unsigned int projLocation = glGetUniformLocation(pShaderProgram, "projTransform");
	unsigned int worldLocation = glGetUniformLocation(pShaderProgram, "worldTransform");
	unsigned int modelLocation = glGetUniformLocation(pShaderProgram, "modelTransform");
	unsigned int colors = glGetUniformLocation(pShaderProgram, "plane_color");

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj_transform));

	world_transform = glm::mat4(1.f);
	world_transform = glm::translate(world_transform, glm::vec3(0.f, -0.4f, 0.f));
	world_transform = glm::rotate(world_transform, glm::radians(30.f), glm::vec3(1.f, 0.f, 0.f));
	world_transform = glm::rotate(world_transform, glm::radians(30.f), glm::vec3(0.f, 1.f, 0.f));
	world_transform = glm::rotate(world_transform, glm::radians(left_turning), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(world_transform));
	
	if (draw_mode)
	{
		front_plane = glm::mat4(1.f);
		model_transform = glm::mat4(1.f);
		front_plane = glm::rotate(front_plane, glm::radians(front_opened), glm::vec3(1.f, 0.f, 0.f));
		model_transform = glm::translate(model_transform, glm::vec3(0.f, 0.f, 0.4f));
		color = glm::vec4(1.f, 0.f, 0.f, 1.f);
		model_transform = model_transform * front_plane;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(world_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(p_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model_transform = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(0.4f, 0.f, 0.f));
		model_transform = glm::rotate(model_transform, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		color = glm::vec4(0.f, 1.f, 0.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(p_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model_transform = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(0.f, 0.f, -0.4f));
		model_transform = glm::rotate(model_transform, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
		color = glm::vec4(0.f, 0.f, 1.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(p_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model_transform = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(-0.4f, 0.f, 0.f));
		model_transform = glm::rotate(model_transform, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
		color = glm::vec4(1.f, 1.f, 0.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(p_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model_transform = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(0.f, 0.f, 0.4f));
		model_transform = glm::rotate(model_transform, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
		color = glm::vec4(1.f, 0.f, 1.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(p_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		up_plane = glm::mat4(1.f);
		model_transform = glm::mat4(1.f);
		up_plane = glm::rotate(up_plane, glm::radians(up_turning), glm::vec3(1.f, 0.f, 0.f));
		up_plane = glm::translate(up_plane, glm::vec3(0.f, -0.4f, 0.f));
		model_transform = glm::translate(model_transform, glm::vec3(0.f, 0.8f, 0.f));
		color = glm::vec4(0.f, 1.f, 1.f, 1.f);
		model_transform = model_transform * up_plane;
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(world_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(p_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else
	{
		///////////////////////////////////////////////
		model_transform = glm::mat4(1.f);
		open_plane = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(0.f, 0.f, -0.4f));
		open_plane = glm::rotate(open_plane, glm::radians(tri_opened), glm::vec3(1.f, 0.f, 0.f));
		model_transform = model_transform * open_plane;
		color = glm::vec4(1.f, 0.f, 0.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(t_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		model_transform = glm::mat4(1.f);
		open_plane = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(0.4f, 0.f, 0.f));
		model_transform = glm::rotate(model_transform, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
		open_plane = glm::rotate(open_plane, glm::radians(tri_opened), glm::vec3(1.f, 0.f, 0.f));
		model_transform = model_transform * open_plane;
		color = glm::vec4(0.f, 1.f, 0.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(t_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		model_transform = glm::mat4(1.f);
		open_plane = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(0.f, 0.f, 0.4f));
		model_transform = glm::rotate(model_transform, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
		open_plane = glm::rotate(open_plane, glm::radians(tri_opened), glm::vec3(1.f, 0.f, 0.f));
		model_transform = model_transform * open_plane;
		color = glm::vec4(0.f, 0.f, 1.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(t_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		model_transform = glm::mat4(1.f);
		open_plane = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(-0.4f, 0.f, 0.f));
		model_transform = glm::rotate(model_transform, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		open_plane = glm::rotate(open_plane, glm::radians(tri_opened), glm::vec3(1.f, 0.f, 0.f));
		model_transform = model_transform * open_plane;
		color = glm::vec4(0.f, 1.f, 1.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(t_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		model_transform = glm::mat4(1.f);
		model_transform = glm::translate(model_transform, glm::vec3(0.f, 0.f, 0.4f));
		model_transform = glm::rotate(model_transform, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
		color = glm::vec4(1.f, 0.f, 1.f, 1.f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform));
		glUniform4fv(colors, 1, glm::value_ptr(color));
		glBindVertexArray(p_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	

	//�׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
		draw_mode = !draw_mode;
		break;
	case 'f':
		front_flag = true;
		break;
	case 'F':
		front_flag = false;
		break;
	case 't':
		up_flag = true;
		break;
	case 'T':
		up_flag = false;
		break;
	case 'y':
		left_turning += 2.5f;
		break;
	case 'Y':
		left_turning -= 2.5f;
		break;
	case 'o':
		open_flag = true;
		break;
	case 'O':
		open_flag = false;
		break;
	case 'p':
		proj_transform = glm::ortho(-2.f, 2.f, -2.f, 2.f, 0.1f, 200.f);
		proj_transform = glm::translate(proj_transform, glm::vec3(0.f, 0.f, -3.f));
		break;
	case 'P':
		proj_transform = glm::perspective(glm::radians(45.f), (800.f / 600.f), 0.1f, 500.f);
		proj_transform = glm::translate(proj_transform, glm::vec3(0.f, 0.f, -3.f));
		break;
	}
}

GLvoid Timer(int value)
{
	if (front_flag)
	{
		if(front_opened<90.f)
			front_opened += 2.f;
	}
	else
	{
		if (front_opened <= 0.f)
		{
			front_opened = 0.f;
		}
		else
		{
			front_opened -= 2.f;
		}
	}

	if (up_flag)
	{
		if (up_turning > 360.f)
		{
			up_turning = 0.f;
			up_turning += 2.f;
		}
		else
		{
			up_turning += 2.f;
		}
	}

	if (open_flag)
	{
		if (tri_opened <= -90.f)
		{
			tri_opened = -90.f;
		}
		else
		{
			tri_opened-=2.f;
		}
	}
	else
	{
		if (tri_opened >= 30.f)
		{
			tri_opened = 30.f;
		}
		else
			tri_opened += 2.f;
	}

	glutTimerFunc(100, Timer, 0);
	glutPostRedisplay();
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
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