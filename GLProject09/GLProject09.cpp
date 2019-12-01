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
void InitCamera();
bool mode = true;
int draw_mode = GLU_LINE;
int timer_var = 0;
float rotrot = 0.f;

glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projM = glm::mat4(1.0f);
glm::mat4 trans_mat[6]
{
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f)
};
glm::mat4 bigRing_rot1 = glm::mat4(1.0f);
glm::mat4 bigRing_rot2 = glm::mat4(1.0f);
glm::mat4 bigRing_rot3 = glm::mat4(1.0f);
glm::mat4 smallRing_rot1 = glm::mat4(1.0f);
glm::mat4 smallRing_rot2 = glm::mat4(1.0f);
glm::mat4 smallRing_rot3 = glm::mat4(1.0f);
glm::mat4 wRotate_mat = glm::mat4(1.0f);

glm::mat4 scale_mat = glm::mat4(1.0f);
glm::mat4 model_transform[9]
{
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f),
	glm::mat4(1.0f)
};
glm::mat4 model = glm::mat4(1.0f);

glm::mat4 world_transform = glm::mat4(1.0f);

glm::mat4 cameraRot = glm::mat4(1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 1.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat small_ring[300];
GLfloat big_ring[300];

float xx = 0;
float yy = 0;
float zz = 0;
float y_rot = 0;

void init_ring();
void init_qobj();

GLuint vao[6], vbo[6];

GLUquadricObj* qobj[7];

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

	init_ring();
	init_qobj();

	trans_mat[0] = glm::translate(trans_mat[0], glm::vec3(0.4f, 0.0f, 0.0f));
	trans_mat[1] = glm::translate(trans_mat[1], glm::vec3(0.4f, 0.0f, 0.0f));
	trans_mat[2] = glm::translate(trans_mat[2], glm::vec3(0.4f, 0.0f, 0.0f));
	trans_mat[3] = glm::translate(trans_mat[3], glm::vec3(0.1f, 0.0f, 0.0f));
	trans_mat[4] = glm::translate(trans_mat[4], glm::vec3(0.1f, 0.0f, 0.0f));
	trans_mat[5] = glm::translate(trans_mat[5], glm::vec3(0.1f, 0.0f, 0.0f));

	bigRing_rot2 = glm::rotate(bigRing_rot1, glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
	bigRing_rot3 = glm::rotate(bigRing_rot1, glm::radians(-45.f), glm::vec3(0.0f, 0.0f, 1.0f));

	glLineWidth(3.0f);

	glGenVertexArrays(6, vao);
	glGenBuffers(6, vbo);

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(small_ring), small_ring, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(small_ring), small_ring, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(small_ring), small_ring, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[3]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(big_ring), big_ring, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[4]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(big_ring), big_ring, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[5]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(big_ring), big_ring, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timer, 1);
	glutMainLoop();

}

void init_ring()
{
	for (int i = 0; i < 300; i += 3)
	{
		small_ring[i] = 0.1 * glm::cos(glm::radians(i * 3.6f));
		small_ring[i + 1] = 0;
		small_ring[i + 2] = 0.1 * glm::sin(glm::radians(i * 3.6f));

		big_ring[i] = 0.4 * glm::cos(glm::radians(i * 3.6f));
		big_ring[i + 1] = 0;
		big_ring[i + 2] = 0.4 * glm::sin(glm::radians(i * 3.6f));
	}
}

void init_qobj()
{
	for (int i = 0; i < 7; ++i)
	{
		qobj[i] = gluNewQuadric();
		gluQuadricDrawStyle(qobj[i], GLU_LINE);
		gluQuadricNormals(qobj[i], GLU_SMOOTH);
		gluQuadricOrientation(qobj[i], GLU_OUTSIDE);
	}
}

GLvoid drawScene()
{
	glUseProgram(pShaderProgram);

	glEnable(GL_CULL_FACE);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	model_transform[0] = bigRing_rot1;
	model_transform[1] = bigRing_rot2;
	model_transform[2] = bigRing_rot3;
	model_transform[3] = bigRing_rot1 * trans_mat[0];
	model_transform[4] = bigRing_rot2 * trans_mat[1];
	model_transform[5] = bigRing_rot3 * trans_mat[2];
	model_transform[6] = model_transform[3] * trans_mat[3];
	model_transform[7] = model_transform[4] * trans_mat[4];
	model_transform[8] = model_transform[5] * trans_mat[5];

	unsigned int modelLocation = glGetUniformLocation(pShaderProgram, "modelTransform");
	unsigned int viewLocation = glGetUniformLocation(pShaderProgram, "viewTransform");
	unsigned int projLocation = glGetUniformLocation(pShaderProgram, "projTransform");
	unsigned int worldLocation = glGetUniformLocation(pShaderProgram, "worldTransform");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	gluSphere(qobj[0], 0.2, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[0]));
	glBindVertexArray(vao[3]);
	glDrawArrays(GL_LINE_STRIP, 0, 100);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[1]));
	glBindVertexArray(vao[4]);
	glDrawArrays(GL_LINE_STRIP, 0, 100);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[2]));
	glBindVertexArray(vao[5]);
	glDrawArrays(GL_LINE_STRIP, 0, 100);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[3]));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINE_STRIP, 0, 100);
	gluSphere(qobj[1], 0.05, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[4]));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_LINE_STRIP, 0, 100);
	gluSphere(qobj[2], 0.05, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[5]));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_LINE_STRIP, 0, 100);
	gluSphere(qobj[3], 0.05, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[6]));
	gluSphere(qobj[4], 0.02, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[7]));
	gluSphere(qobj[5], 0.02, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model_transform[8]));
	gluSphere(qobj[6], 0.02, 20, 20);

	projM = glm::mat4(1.0f);
	projM = glm::perspective(glm::radians(60.0f), 800.f / 600.f, 0.1f, 100.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projM));

	world_transform = glm::mat4(1.f);
	world_transform = glm::translate(world_transform, glm::vec3(xx, yy, zz));
	world_transform = glm::rotate(world_transform, glm::radians(y_rot), glm::vec3(0.f, 1.f, 0.f));

	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(world_transform));

	//그리기 부분 구현


	//그리기 관련 부분이 여기에 포함된다.
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
	case 'w':
		yy -= 0.2f;
		break;
	case 's':
		yy += 0.2f;
		break;
	case 'a':
		xx -= 0.2f;
		break;
	case 'd':
		xx += 0.2f;
		break;
	case 'z':
		zz -= 0.2f;
		break;
	case 'x':
		zz += 0.2f;
		break;
	case 'y':
		y_rot += 3.5f;
		break;
	case 'Y':
		y_rot -= 3.5f;
		break;
	}


	glutPostRedisplay();
}

GLvoid Timer(int value)
{
	++timer_var;
	glm::vec3 big_spin[3];
	glm::vec3 small_spin[3];

	big_spin[0] = glm::vec3(0.4 * glm::cos(glm::radians(timer_var * 3.6f)), 0, 0.4 * glm::sin(glm::radians(timer_var * 3.6f)));
	big_spin[1] = glm::vec3(0.4 * glm::cos(glm::radians(timer_var * 4.8f)), 0, 0.4 * glm::sin(glm::radians(timer_var * 4.8f)));
	big_spin[2] = glm::vec3(0.4 * glm::cos(glm::radians(timer_var * 9.2f)), 0, 0.4 * glm::sin(glm::radians(timer_var * 9.2f)));

	small_spin[0] = glm::vec3(0.1 * glm::cos(glm::radians(timer_var * 5.6f)), 0, 0.1 * glm::sin(glm::radians(timer_var * 5.6f)));
	small_spin[1] = glm::vec3(0.1 * glm::cos(glm::radians(timer_var * 8.8f)), 0, 0.1 * glm::sin(glm::radians(timer_var * 8.8f)));
	small_spin[2] = glm::vec3(0.1 * glm::cos(glm::radians(timer_var * 12.2f)), 0, 0.1 * glm::sin(glm::radians(timer_var * 12.2f)));

	trans_mat[0] = glm::mat4(1.0f);
	trans_mat[1] = glm::mat4(1.0f);
	trans_mat[2] = glm::mat4(1.0f);
	trans_mat[3] = glm::mat4(1.0f);
	trans_mat[4] = glm::mat4(1.0f);
	trans_mat[5] = glm::mat4(1.0f);

	trans_mat[0] = glm::translate(trans_mat[0], big_spin[0]);
	trans_mat[1] = glm::translate(trans_mat[1], big_spin[1]);
	trans_mat[2] = glm::translate(trans_mat[2], big_spin[2]);
	trans_mat[3] = glm::translate(trans_mat[3], small_spin[0]);
	trans_mat[4] = glm::translate(trans_mat[4], small_spin[1]);
	trans_mat[5] = glm::translate(trans_mat[5], small_spin[2]);

	glutTimerFunc(100, Timer, 1);
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