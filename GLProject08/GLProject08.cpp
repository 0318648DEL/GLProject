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
GLvoid Keyboard_s(int key, int x, int y);
int mode = 1;
int draw_mode = GLU_LINE;

glm::mat4 trans_mat = glm::mat4(1.0f);
glm::mat4 mRotate_mat1 = glm::mat4(1.0f);
glm::mat4 mRotate_mat2 = glm::mat4(1.0f);
glm::mat4 wRotate_mat = glm::mat4(1.0f);
glm::mat4 model1_transform = glm::mat4(1.0f);
glm::mat4 model2_transform = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

GLuint vao, vbo[2];

GLfloat lines[]
{
	-1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.0f,-1.0f,0.0f,
	0.0f,1.0f,0.0f
};

GLfloat color[]
{
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f
};

GLUquadricObj* qobj;

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
	glGenBuffers(2, vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Keyboard_s);
	glutMainLoop();

}

GLvoid drawScene()
{

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pShaderProgram);


	//그리기 부분 구현
	unsigned int modelLocation = glGetUniformLocation(pShaderProgram, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int projLocation = glGetUniformLocation(pShaderProgram, "projTransform");

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 4);

	model1_transform = wRotate_mat * trans_mat * mRotate_mat1;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model1_transform));

	switch (mode)
	{
	case 1:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, draw_mode);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.4, 50, 50);
		break;
	case 2:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, draw_mode);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluCylinder(qobj, 0.4, 0.0, 0.6, 50, 8);
		break;
	case 3:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, draw_mode);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluCylinder(qobj, 0.4, 0.0, 0.6, 50, 8);
		break;
	case 4:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, draw_mode);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluDisk(qobj, 0.2, 0.5, 20, 20);
		break;
	case 5:
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, draw_mode);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluCylinder(qobj, 0.5, 0.5, 0.8, 50, 8);
		break;
	}


	glm::mat4 projM = glm::mat4(1.0f);
	projM = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projM));



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
	case '1':
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		mRotate_mat = glm::mat4(1.0f);
		mode = 1;
		break;
	case '2':
		up = glm::vec3(0.0f, 0.0f, 1.0f);
		mRotate_mat = glm::mat4(1.0f);
		mRotate_mat = glm::rotate(mRotate_mat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		mode = 2;
		break;
	case '3':
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		mRotate_mat = glm::mat4(1.0f);
		mRotate_mat = glm::rotate(mRotate_mat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		mode = 3;
		break;
	case '4':
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		mRotate_mat = glm::mat4(1.0f);
		mode = 4;
		break;
	case '5':
		up = glm::vec3(0.0f, 0.0f, 1.0f);
		mRotate_mat = glm::mat4(1.0f);
		mRotate_mat = glm::rotate(mRotate_mat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		mode = 5;
		break;
	case 'w':
		draw_mode = GLU_LINE;
		break;
	case 'W':
		draw_mode = GLU_FILL;
		break;
	case 'y':
		mRotate_mat = glm::rotate(mRotate_mat, glm::radians(3.5f), up);
		break;
	case 'Y':
		mRotate_mat = glm::rotate(mRotate_mat, glm::radians(-3.5f), up);
		break;
	case 'r':
		wRotate_mat = glm::rotate(wRotate_mat, glm::radians(3.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'R':
		wRotate_mat = glm::rotate(wRotate_mat, glm::radians(-3.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'c':
		wRotate_mat = glm::mat4(1.0f);
		mRotate_mat = glm::mat4(1.0f);
		trans_mat = glm::mat4(1.0f);
		switch (mode)
		{
		case 2:
			up = glm::vec3(0.0f, 0.0f, 1.0f);
			mRotate_mat = glm::mat4(1.0f);
			mRotate_mat = glm::rotate(mRotate_mat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		case 3:
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			mRotate_mat = glm::mat4(1.0f);
			mRotate_mat = glm::rotate(mRotate_mat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 5:
			up = glm::vec3(0.0f, 0.0f, 1.0f);
			mRotate_mat = glm::mat4(1.0f);
			mRotate_mat = glm::rotate(mRotate_mat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		}
		break;
	}

	glutPostRedisplay();
}

GLvoid Keyboard_s(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		trans_mat = glm::translate(trans_mat, glm::vec3(-0.05f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_RIGHT:
		trans_mat = glm::translate(trans_mat, glm::vec3(0.05f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_UP:
		trans_mat = glm::translate(trans_mat, glm::vec3(0.0f, 0.05f, 0.0f));
		break;
	case GLUT_KEY_DOWN:
		trans_mat = glm::translate(trans_mat, glm::vec3(0.0f, -0.05f, 0.0f));
		break;
	}

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