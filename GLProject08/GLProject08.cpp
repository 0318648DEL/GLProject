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
bool mode = true;
int draw_mode = GLU_LINE;

glm::mat4 trans_mat1 = glm::mat4(1.0f);
glm::mat4 trans_mat2 = glm::mat4(1.0f);
glm::mat4 mRotate_mat1 = glm::mat4(1.0f);
glm::mat4 mRotate_mat2 = glm::mat4(1.0f);
glm::mat4 wRotate_mat = glm::mat4(1.0f);
glm::mat4 scale_mat = glm::mat4(1.0f);
glm::mat4 model1_transform = glm::mat4(1.0f);
glm::mat4 model2_transform = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

GLuint vao[3], vbo[6];
GLuint c_ebo, p_ebo;

GLfloat lines[]
{
	-1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.0f,-1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,-1.0f,
	0.0f,0.0f,1.0f
};

GLfloat color[]
{
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f
};

GLfloat c_pos[]
{
	-0.4f,-0.4f,0.4f,
	0.4f,-0.4f,0.4f,
	0.4f,0.4f,0.4f,
	-0.4f,0.4f,0.4f,
	-0.4f,-0.4f,-0.4f,
	0.4f,-0.4f,-0.4f,
	0.4f,0.4f,-0.4f,
	-0.4f,0.4f,-0.4f
};

GLfloat c_color[]
{
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f,
	1.0f,1.0f,1.0f,
	0.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f,
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

GLfloat p_pos[]
{
	-0.4f,-0.4f,0.4f,
	0.4f,-0.4f,0.4f,
	0.4f,-0.4f,-0.4f,
	-0.4f,-0.4f,-0.4f,
	0.0f,0.4f,0.0f
};

GLfloat p_color[]
{
	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f,
	1.0f,1.0f,1.0f,
	0.0f,0.0f,0.0f,
};

unsigned int p_index[]
{
	0,1,4,
	1,2,4,
	2,3,4,
	3,0,4,
	3,2,0,
	1,0,2
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

	glGenVertexArrays(3, vao);
	glGenBuffers(6, vbo);
	glGenBuffers(1, &c_ebo);
	glGenBuffers(1, &p_ebo);

	scale_mat = glm::scale(scale_mat, glm::vec3(0.5f, 0.5f, 0.5f));
	wRotate_mat = glm::rotate(wRotate_mat, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	trans_mat1 = glm::translate(trans_mat1, glm::vec3(-0.5f, 0.0f, 0.0f));
	trans_mat2 = glm::translate(trans_mat2, glm::vec3(0.5f, 0.0f, 0.0f));

	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(c_pos), c_pos, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(c_index), c_index, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(c_color), c_color, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(c_index), c_index, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_pos), p_pos, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p_index), p_index, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_color), p_color, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p_index), p_index, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

}

GLvoid drawScene()
{
	glEnable(GL_CULL_FACE);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pShaderProgram);

	model = wRotate_mat;
	//그리기 부분 구현
	unsigned int modelLocation = glGetUniformLocation(pShaderProgram, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int projLocation = glGetUniformLocation(pShaderProgram, "projTransform");

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 6);

	model1_transform = wRotate_mat * trans_mat1 * mRotate_mat1 * scale_mat;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model1_transform));

	glBindVertexArray(vao[1]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model2_transform = wRotate_mat * trans_mat2 * mRotate_mat2 * scale_mat;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model2_transform));

	glBindVertexArray(vao[2]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);


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
	case 'y':
		mRotate_mat2 = glm::rotate(mRotate_mat2, glm::radians(3.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'Y':
		mRotate_mat2 = glm::rotate(mRotate_mat2, glm::radians(-3.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'x':
		mRotate_mat1 = glm::rotate(mRotate_mat1, glm::radians(3.5f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case 'X':
		mRotate_mat1 = glm::rotate(mRotate_mat1, glm::radians(-3.5f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case 'b':
		wRotate_mat = glm::rotate(wRotate_mat, glm::radians(3.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'B':
		wRotate_mat = glm::rotate(wRotate_mat, glm::radians(-3.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'c':
		glm::mat4 temp = trans_mat2;
		trans_mat2 = trans_mat1;
		trans_mat1 = temp;
		temp = mRotate_mat2;
		mRotate_mat2 = mRotate_mat1;
		mRotate_mat1 = temp;
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