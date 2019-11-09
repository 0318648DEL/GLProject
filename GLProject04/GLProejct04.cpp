#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> dis(0, 1);
std::uniform_real_distribution<float> color(0.f, 1.f);

class Circle
{
public:

	GLfloat pos[3];
	GLfloat vertices[20][3];
	GLfloat colors[20][3];
	GLfloat rad;

	float radian = 3.1415926f / 180;
	int draw_mode = -1;
	void init_circle(float x, float y);
	void increase_rad();
};

void Circle::init_circle(float x, float y)
{
	pos[0] = x, pos[1] = y, pos[2] = 0;
	rad = 0.05;
	draw_mode = dis(engine);

	for (int i = 0; i < 20; ++i)
	{
		vertices[i][0] = pos[0]+(rad * cosf((18.f*i) * radian));
		vertices[i][1] = pos[1]+(rad * sinf((18.f*i) * radian));
		vertices[i][2] = 0;

		colors[i][0] = color(engine);
		colors[i][1] = color(engine);
		colors[i][2] = color(engine);
	}
}

void Circle::increase_rad()
{
	if (rad < 2.0f)
	{
		rad += 0.05f;
		for (int i = 0; i < 20; ++i)
		{
			vertices[i][0] = pos[0] + (rad * cosf((18.f * i) * radian));
			vertices[i][1] = pos[1] + (rad * sinf((18.f * i) * radian));
			vertices[i][2] = 0;
		}
	}
	else if (draw_mode>0)
	{
		rad = 0.05f;
		for (int i = 0; i < 20; ++i)
		{
			vertices[i][0] = pos[0] + (rad * cosf((18.f * i) * radian));
			vertices[i][1] = pos[1] + (rad * sinf((18.f * i) * radian));
			vertices[i][2] = 0;
		}
	}
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
bool ReadFile(char* filename, std::string* target);
GLvoid BindBuffer(GLuint vao, GLuint v_vbo, GLuint c_vbo, GLint vpAttribute, GLint cpAttribute, const Circle data);
GLuint CompileShaders(const char* filenameVS, const char* filenameFS);
GLuint pShaderProgram;
GLvoid drawScene();
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Reshape(int w, int h);

int mode = GL_LINE_LOOP;
int unproper_var = 0;
GLuint vao[10], vbo[20];
Circle circles[10];

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example03");

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

	glGenVertexArrays(10, vao);
	glGenBuffers(20, vbo);
	glPointSize(7);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, Timer, 1);
	glutMainLoop();
}

GLvoid drawScene()
{

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//그리기 부분 구현
	for (int i = 0; i < 10; ++i)
	{
		if (circles[i].draw_mode < 0)
			break;
		glBindVertexArray(vao[i]);
		glDrawArrays(mode, 0, 20);
	}
	//그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Timer(int value)
{
	for (int i = 0; i < 10; ++i)
	{
		if (circles[i].draw_mode < 0)
			break;
		circles[i].increase_rad();
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(circles[i].vertices), circles[i].vertices);
	}
	glutPostRedisplay();
	glutTimerFunc(100, Timer, 1);

}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		unproper_var = unproper_var % 10;
		float x_pos = (x - (800.f / 2)) / (800.f / 2);
		float y_pos = -(y - (600.f / 2)) / (600.f / 2);

		if (circles[unproper_var].draw_mode<0)
		{
			circles[unproper_var].init_circle(x_pos, y_pos);
			BindBuffer(vao[unproper_var], vbo[unproper_var], vbo[unproper_var + 10], 0, 1, circles[unproper_var]);
		}
		else
		{
			circles[unproper_var].init_circle(x_pos, y_pos);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[unproper_var]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(circles[unproper_var].vertices), circles[unproper_var].vertices);
		}

		++unproper_var;
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'i':
		mode = GL_POINTS;
		break;
	case 'j':
		mode = GL_LINE_LOOP;
		break;
	}
}


GLvoid BindBuffer(GLuint vao, GLuint v_vbo, GLuint c_vbo, GLint vpAttribute, GLint cpAttribute, const Circle data)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, v_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data.vertices), data.vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vpAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(vpAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, c_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data.colors), data.colors, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(cpAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(cpAttribute);
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
