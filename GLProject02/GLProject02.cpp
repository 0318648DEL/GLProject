#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);



class triangle
{
private:
	int turn_mode = 0;

public:
	GLfloat color[3][3];
	GLfloat vertices[3][3];

	triangle();
	triangle(float x, float y, float z);
	void set_pos(float x, float y, float z);
	void move(float x, float y);
	void change_pos(float x, float y, float z);
	void change_color();

};

triangle::triangle()
{

}

triangle::triangle(float x, float y, float z)
{
	vertices[0][0] = x + 0.1;
	vertices[0][1] = y - 0.1;
	vertices[0][2] = z;

	vertices[1][0] = x - 0.1;
	vertices[1][1] = y - 0.1;
	vertices[1][2] = z;

	vertices[2][0] = x;
	vertices[2][1] = y + 0.1;
	vertices[2][2] = z;

	color[0][0] = dis(engine);
	color[0][1] = dis(engine);
	color[0][2] = dis(engine);

	color[1][0] = dis(engine);
	color[1][1] = dis(engine);
	color[1][2] = dis(engine);

	color[2][0] = dis(engine);
	color[2][1] = dis(engine);
	color[2][2] = dis(engine);
}

void triangle::move(float x, float y)
{
	if (turn_mode==0)
	{
		if ((vertices[1][0] - x*2) < -1.f)
			turn_mode = 1;
		vertices[0][0] -= x;
		vertices[1][0] -= x;
		vertices[2][0] -= x;
	}
	else if (turn_mode==1)
	{
		if ((vertices[2][1] + y*2) > 1.f)
			turn_mode = 2;
		vertices[0][1] += y;
		vertices[1][1] += y;
		vertices[2][1] += y;
	}
	else if (turn_mode==2)
	{
		if ((vertices[0][0] + x*2) > 1.f)
			turn_mode = 3;
		vertices[0][0] += x;
		vertices[1][0] += x;
		vertices[2][0] += x;
	}
	else if (turn_mode==3)
	{
		if ((vertices[0][1] - y*2) < -1.f)
			turn_mode = 0;
		vertices[0][1] -= y;
		vertices[1][1] -= y;
		vertices[2][1] -= y;
	}
}

void triangle::set_pos(float x, float y, float z)
{
	vertices[0][0] = x + 0.1;
	vertices[0][1] = y - 0.1;
	vertices[0][2] = z;

	vertices[1][0] = x - 0.1;
	vertices[1][1] = y - 0.1;
	vertices[1][2] = z;

	vertices[2][0] = x;
	vertices[2][1] = y + 0.1;
	vertices[2][2] = z;

	color[0][0] = dis(engine);
	color[0][1] = dis(engine);
	color[0][2] = dis(engine);

	color[1][0] = dis(engine);
	color[1][1] = dis(engine);
	color[1][2] = dis(engine);

	color[2][0] = dis(engine);
	color[2][1] = dis(engine);
	color[2][2] = dis(engine);
}

void triangle::change_pos(float x, float y, float z)
{
	set_pos(x, y, z);
}

void triangle::change_color()
{
	color[0][0] = dis(engine);
	color[0][1] = dis(engine);
	color[0][2] = dis(engine);

	color[1][0] = dis(engine);
	color[1][1] = dis(engine);
	color[1][2] = dis(engine);

	color[2][0] = dis(engine);
	color[2][1] = dis(engine);
	color[2][2] = dis(engine);
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
bool ReadFile(char* filename, std::string* target);
GLvoid BindBuffer(GLuint vao, GLuint v_vbo, GLuint c_vbo, GLint vpAttribute, GLint cpAttribute, const triangle data);

int window_width = 800;
int window_height = 600;
int unproper_var = 0;
int polymode = 0;
bool break_point = true;

GLuint vao[4], vbo[8];
GLuint CompileShaders(const char* filenameVS, const char* filenameFS);
GLuint pShaderProgram;
GLvoid drawScene();
GLvoid Mouse(int x, int y, int width, int height);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Reshape(int w, int h);
triangle triangles[4];

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


	triangles[0].set_pos(-0.5, 0.5, 0.0);
	triangles[1].set_pos(-0.5, -0.5, 0.0);
	triangles[2].set_pos(0.5, 0.5, 0.0);
	triangles[3].set_pos(0.5, -0.5, 0.0);
	glGenVertexArrays(4, vao);
	glGenBuffers(8, vbo);
	BindBuffer(vao[0], vbo[0], vbo[4], 0, 1, triangles[0]);
	BindBuffer(vao[1], vbo[1], vbo[5], 0, 1, triangles[1]);
	BindBuffer(vao[2], vbo[2], vbo[6], 0, 1, triangles[2]);
	BindBuffer(vao[3], vbo[3], vbo[7], 0, 1, triangles[3]);

	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//그리기 부분 구현
	for (int i = 0; i < 4; ++i)
	{
		glBindVertexArray(vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	//그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers();
}

GLvoid BindBuffer(GLuint vao, GLuint v_vbo, GLuint c_vbo, GLint vpAttribute, GLint cpAttribute, const triangle data)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, v_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data.vertices), data.vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vpAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(vpAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, c_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data.color), data.color, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(cpAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(cpAttribute);
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON&&state==GLUT_UP)
	{
		unproper_var = unproper_var % 4;

		float x_pos = (x - (static_cast<float>(window_width) / 2)) / (static_cast<float>(window_width)/2);
		float y_pos = -(y - (static_cast<float>(window_height) / 2)) / (static_cast<float>(window_height)/2);

		triangles[unproper_var].set_pos(x_pos, y_pos, 0.0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[unproper_var]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangles[unproper_var].vertices), triangles[unproper_var].vertices);
		++unproper_var;
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		if (!polymode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			polymode = 1;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			polymode = 0;
		}
		glutPostRedisplay();
		break;
	case 'm':
		break_point = false;
		glutTimerFunc(10, Timer, 1);
		break;
	case 's':
		break_point = true;
		break;
	case 'c':
		for (int i = 0; i < 4; ++i)
		{
			triangles[i].change_color();
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i+4]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangles[i].color), triangles[i].color);
		}
		glutPostRedisplay();
		break;
	case 'q':
		exit(0);
	}
}

GLvoid Timer(int value)
{
	if (!break_point)
	{
		for (int i = 0; i < 4; ++i)
		{
			triangles[i].move(0.05, 0.05);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangles[i].vertices), triangles[i].vertices);
		}
		glutPostRedisplay();
		glutTimerFunc(10, Timer, 1);
	}

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