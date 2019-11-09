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
std::uniform_real_distribution<float> dis(0.0f, 1.0f);
std::uniform_real_distribution<float> bounce(-0.15f, 0.15f);
std::uniform_real_distribution<float> pos(-0.8f, 0.8f);

class triangle
{
private:
	int turn_mode = 0;
	GLfloat bound_x;
	GLfloat bound_y;

public:
	GLfloat color[3][3];
	GLfloat vertices[3][3];

	triangle();
	triangle(float x, float y, float z);
	void set_pos(float x, float y, float z);
	void move();
	void change_pos(float x, float y, float z);
	void change_color();

};

triangle::triangle()
{

}

triangle::triangle(float x, float y, float z)
{
	bound_x = bounce(engine);
	bound_y = bounce(engine);

	vertices[0][0] = x + 0.1;
	vertices[0][1] = y - 0.1;
	vertices[0][2] = z;

	vertices[1][0] = x - 0.1;
	vertices[1][1] = y - 0.1;
	vertices[1][2] = z;

	vertices[2][0] = x;
	vertices[2][1] = y + 0.2;
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

void triangle::move()
{

	if ((vertices[0][0] > -0.5f && vertices[1][0] > -0.5f && vertices[2][0] > -0.5f)
		&& (vertices[0][1] > -0.5f && vertices[1][1] > -0.5f && vertices[2][1] > -0.5f)
		&& (vertices[0][0] < 0.5f && vertices[1][0] < 0.5f && vertices[2][0] < 0.5f)
		&& (vertices[0][1] < 0.5f && vertices[1][1] < 0.5f && vertices[2][1] < 0.5f))
	{
		if ((vertices[0][0] + bound_x * 1.5f) < -0.5f)
		{
			vertices[0][1] -= 0.1;
			vertices[1][0] = vertices[0][0];
			vertices[1][1] = vertices[0][1] + 0.2;
			vertices[2][0] = vertices[0][0] + 0.3;
			vertices[2][1] = vertices[0][1] + 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][0] + bound_x * 1.5f) < -0.5f)
		{
			vertices[1][1] += 0.1;
			vertices[0][0] = vertices[1][0];
			vertices[0][1] = vertices[1][1] - 0.2;
			vertices[2][0] = vertices[1][0] + 0.3;
			vertices[2][1] = vertices[1][1] - 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][0] + bound_x * 1.5f) < -0.5f)
		{
			vertices[0][0] = vertices[2][0];
			vertices[1][0] = vertices[2][0];
			vertices[0][1] = vertices[2][1] - 0.1;
			vertices[1][1] = vertices[2][1] + 0.1;
			vertices[2][0] += 0.3;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[0][1] + bound_y * 1.5f) > 0.5f)
		{
			vertices[0][0] -= 0.1;
			vertices[1][0] = vertices[0][0] + 0.2;
			vertices[1][1] = vertices[0][1];
			vertices[2][0] = vertices[0][0] + 0.1;
			vertices[2][1] = vertices[0][1] - 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][1] + bound_y * 1.5f) > 0.5f)
		{
			vertices[1][0] += 0.1;
			vertices[0][0] = vertices[1][0] - 0.2;
			vertices[0][1] = vertices[1][1];
			vertices[2][0] = vertices[1][0] - 0.1;
			vertices[2][1] = vertices[1][1] - 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][1] + bound_y * 1.5f) > 0.5f)
		{
			vertices[0][1] = vertices[2][1];
			vertices[1][1] = vertices[2][1];
			vertices[0][0] = vertices[2][0] - 0.1;
			vertices[1][0] = vertices[2][0] + 0.1;
			vertices[2][1] -= 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[0][0] + bound_x * 1.5f) > 0.5f)
		{
			vertices[0][1] += 0.1;
			vertices[1][0] = vertices[0][0];
			vertices[1][1] = vertices[0][1] - 0.2;
			vertices[2][0] = vertices[0][0] - 0.3;
			vertices[2][1] = vertices[0][1] - 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][0] + bound_x * 1.5f) > 0.5f)
		{
			vertices[1][1] -= 0.1;
			vertices[0][0] = vertices[1][0];
			vertices[0][1] = vertices[1][1] + 0.2;
			vertices[2][0] = vertices[1][0] - 0.3;
			vertices[2][1] = vertices[1][1] + 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][0] + bound_x * 1.5f) > 0.5f)
		{
			vertices[1][0] = vertices[2][0];
			vertices[0][0] = vertices[2][0];
			vertices[1][1] = vertices[2][1] - 0.1;
			vertices[0][1] = vertices[2][1] + 0.1;
			vertices[2][0] -= 0.3;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[0][1] + bound_y * 1.5f) < -0.5f)
		{
			vertices[0][0] += 0.1;
			vertices[1][0] = vertices[0][0] - 0.2;
			vertices[1][1] = vertices[0][1];
			vertices[2][0] = vertices[0][0] - 0.1;
			vertices[2][1] = vertices[0][1] + 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][1] + bound_y * 1.5f) < -0.5f)
		{
			vertices[1][0] -= 0.1;
			vertices[0][0] = vertices[1][0] + 0.2;
			vertices[0][1] = vertices[1][1];
			vertices[2][0] = vertices[1][0] + 0.1;
			vertices[2][1] = vertices[1][1] + 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][1] + bound_y * 1.5f) < -0.5f)
		{
			vertices[0][1] = vertices[2][1];
			vertices[1][1] = vertices[2][1];
			vertices[0][0] = vertices[2][0] + 0.1;
			vertices[1][0] = vertices[2][0] - 0.1;
			vertices[2][1] += 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
	}
	else
	{
		if ((vertices[0][0] + bound_x * 1.5f) < -1.0f)
		{
			vertices[0][1] -= 0.1;
			vertices[1][0] = vertices[0][0];
			vertices[1][1] = vertices[0][1] + 0.2;
			vertices[2][0] = vertices[0][0] + 0.3;
			vertices[2][1] = vertices[0][1] + 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][0] + bound_x * 1.5f) < -1.0f)
		{
			vertices[1][1] += 0.1;
			vertices[0][0] = vertices[1][0];
			vertices[0][1] = vertices[1][1] - 0.2;
			vertices[2][0] = vertices[1][0] + 0.3;
			vertices[2][1] = vertices[1][1] - 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][0] + bound_x * 1.5f) < -1.0f)
		{
			vertices[0][0] = vertices[2][0];
			vertices[1][0] = vertices[2][0];
			vertices[0][1] = vertices[2][1] - 0.1;
			vertices[1][1] = vertices[2][1] + 0.1;
			vertices[2][0] += 0.3;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[0][1] + bound_y * 1.5f) > 1.0f)
		{
			vertices[0][0] -= 0.1;
			vertices[1][0] = vertices[0][0] + 0.2;
			vertices[1][1] = vertices[0][1];
			vertices[2][0] = vertices[0][0] + 0.1;
			vertices[2][1] = vertices[0][1] - 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][1] + bound_y * 1.5f) > 1.0f)
		{
			vertices[1][0] += 0.1;
			vertices[0][0] = vertices[1][0] - 0.2;
			vertices[0][1] = vertices[1][1];
			vertices[2][0] = vertices[1][0] - 0.1;
			vertices[2][1] = vertices[1][1] - 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][1] + bound_y * 1.5f) > 1.0f)
		{
			vertices[0][1] = vertices[2][1];
			vertices[1][1] = vertices[2][1];
			vertices[0][0] = vertices[2][0] - 0.1;
			vertices[1][0] = vertices[2][0] + 0.1;
			vertices[2][1] -= 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[0][0] + bound_x * 1.5f) > 1.0f)
		{
			vertices[0][1] += 0.1;
			vertices[1][0] = vertices[0][0];
			vertices[1][1] = vertices[0][1] - 0.2;
			vertices[2][0] = vertices[0][0] - 0.3;
			vertices[2][1] = vertices[0][1] - 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][0] + bound_x * 1.5f) > 1.0f)
		{
			vertices[1][1] -= 0.1;
			vertices[0][0] = vertices[1][0];
			vertices[0][1] = vertices[1][1] + 0.2;
			vertices[2][0] = vertices[1][0] - 0.3;
			vertices[2][1] = vertices[1][1] + 0.1;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][0] + bound_x * 1.5f) > 1.0f)
		{
			vertices[1][0] = vertices[2][0];
			vertices[0][0] = vertices[2][0];
			vertices[1][1] = vertices[2][1] - 0.1;
			vertices[0][1] = vertices[2][1] + 0.1;
			vertices[2][0] -= 0.3;
			bound_x *= -1.f;
			this->change_color();

		}
		else if ((vertices[0][1] + bound_y * 1.5f) < -1.0f)
		{
			vertices[0][0] += 0.1;
			vertices[1][0] = vertices[0][0] - 0.2;
			vertices[1][1] = vertices[0][1];
			vertices[2][0] = vertices[0][0] - 0.1;
			vertices[2][1] = vertices[0][1] + 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[1][1] + bound_y * 1.5f) < -1.0f)
		{
			vertices[1][0] -= 0.1;
			vertices[0][0] = vertices[1][0] + 0.2;
			vertices[0][1] = vertices[1][1];
			vertices[2][0] = vertices[1][0] + 0.1;
			vertices[2][1] = vertices[1][1] + 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if ((vertices[2][1] + bound_y * 1.5f) < -1.0f)
		{
			vertices[0][1] = vertices[2][1];
			vertices[1][1] = vertices[2][1];
			vertices[0][0] = vertices[2][0] + 0.1;
			vertices[1][0] = vertices[2][0] - 0.1;
			vertices[2][1] += 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if (((vertices[2][0] + bound_x) < 0.5f) && vertices[2][0] > 0.5f && ((vertices[2][1] > -0.5f) || (vertices[2][1] < 0.5f)))
		{
			vertices[0][0] = vertices[2][0];
			vertices[1][0] = vertices[2][0];
			vertices[0][1] = vertices[2][1] - 0.1;
			vertices[1][1] = vertices[2][1] + 0.1;
			vertices[2][0] += 0.3;
			bound_x *= -1.f;
			this->change_color();

		}
		else if (((vertices[2][1] + bound_y) > -0.5f) && vertices[2][1] < -0.5f && ((vertices[2][0] > -0.5f) || (vertices[2][0] < 0.5f)))
		{
			vertices[0][1] = vertices[2][1];
			vertices[1][1] = vertices[2][1];
			vertices[0][0] = vertices[2][0] - 0.1;
			vertices[1][0] = vertices[2][0] + 0.1;
			vertices[2][1] -= 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
		else if (((vertices[2][0] + bound_x) > -0.5f) && vertices[2][0] < -0.5f && ((vertices[2][1] > -0.5f) || (vertices[2][1] < 0.5f)))
		{
			vertices[1][0] = vertices[2][0];
			vertices[0][0] = vertices[2][0];
			vertices[1][1] = vertices[2][1] - 0.1;
			vertices[0][1] = vertices[2][1] + 0.1;
			vertices[2][0] -= 0.3;
			bound_x *= -1.f;
			this->change_color();

		}
		else if (((vertices[2][1] + bound_y) < 0.5f) && vertices[2][1] > 0.5f && ((vertices[2][0] > -0.5f) || (vertices[2][0] < 0.5f)))
		{
			vertices[0][1] = vertices[2][1];
			vertices[1][1] = vertices[2][1];
			vertices[0][0] = vertices[2][0] + 0.1;
			vertices[1][0] = vertices[2][0] - 0.1;
			vertices[2][1] += 0.3;
			bound_y *= -1.f;
			this->change_color();

		}
	}


	for (int i = 0; i < 3; ++i)
	{
		vertices[i][0] += bound_x;
		vertices[i][1] += bound_y;
	}

}

void triangle::set_pos(float x, float y, float z)
{
	bound_x = bounce(engine);
	bound_y = bounce(engine);

	vertices[0][0] = x - 0.1;
	vertices[0][1] = y - 0.1;
	vertices[0][2] = z;

	vertices[1][0] = x + 0.1;
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
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			color[i][j] = dis(engine);
		}
	}
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
bool ReadFile(char* filename, std::string* target);
GLvoid BindBuffer(GLuint vao, GLuint v_vbo, GLuint c_vbo, GLint vpAttribute, GLint cpAttribute, const triangle data);
GLuint CompileShaders(const char* filenameVS, const char* filenameFS);
GLuint pShaderProgram;
GLvoid drawScene();
GLvoid Timer(int value);
GLvoid Reshape(int w, int h);
GLuint vao[10], vbo[20];
GLuint rect_vao;
GLuint rect_vbo[2];
GLfloat rect[] = {
	-0.5f,-0.5f,0.0f,-0.5f,0.5f,0.0f,0.5f,0.5f,0.0f,0.5f,-0.5f,0.0f
};
triangle triangles[10];

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example03");

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

	triangles[0].set_pos(-0.1f, 0.1f, 0);
	triangles[1].set_pos(0.1f, -0.1f, 0);
	triangles[2].set_pos(pos(engine), pos(engine), 0);
	triangles[3].set_pos(pos(engine), pos(engine), 0);
	triangles[4].set_pos(pos(engine), pos(engine), 0);
	triangles[5].set_pos(pos(engine), pos(engine), 0);
	triangles[6].set_pos(pos(engine), pos(engine), 0);
	triangles[7].set_pos(pos(engine), pos(engine), 0);
	triangles[8].set_pos(pos(engine), pos(engine), 0);
	triangles[9].set_pos(pos(engine), pos(engine), 0);
	glGenVertexArrays(10, vao);
	glGenBuffers(20, vbo);
	glGenVertexArrays(1, &rect_vao);
	glGenBuffers(2, rect_vbo);
	{
		glBindVertexArray(rect_vao);

		glBindBuffer(GL_ARRAY_BUFFER, rect_vbo[0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, rect_vbo[1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
	}
	BindBuffer(vao[0], vbo[0], vbo[10], 0, 1, triangles[0]);
	BindBuffer(vao[1], vbo[1], vbo[11], 0, 1, triangles[1]);
	BindBuffer(vao[2], vbo[2], vbo[12], 0, 1, triangles[2]);
	BindBuffer(vao[3], vbo[3], vbo[13], 0, 1, triangles[3]);
	BindBuffer(vao[4], vbo[4], vbo[14], 0, 1, triangles[4]);
	BindBuffer(vao[5], vbo[5], vbo[15], 0, 1, triangles[5]);
	BindBuffer(vao[6], vbo[6], vbo[16], 0, 1, triangles[6]);
	BindBuffer(vao[7], vbo[7], vbo[17], 0, 1, triangles[7]);
	BindBuffer(vao[8], vbo[8], vbo[18], 0, 1, triangles[8]);
	BindBuffer(vao[9], vbo[9], vbo[19], 0, 1, triangles[9]);
	glutTimerFunc(100, Timer, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//�׸��� �κ� ����
	for (int i = 0; i < 10; ++i)
	{
		glBindVertexArray(vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	glBindVertexArray(rect_vao);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	//�׸��� ���� �κ��� ���⿡ ���Եȴ�.
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
		triangles[i].move();
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangles[i].vertices), triangles[i].vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i + 10]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangles[i].color), triangles[i].color);
	}
	glutPostRedisplay();
	glutTimerFunc(100, Timer, 1);

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