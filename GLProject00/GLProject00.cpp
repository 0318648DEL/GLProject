#include <iostream>
#include <random>
#include <chrono>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLfloat cColor[4]{ 1.0f,1.0f,1.0f,1.0f };
std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);
bool break_p = true;

void main(int argc, char** argv)
{
	//윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");

	//GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialization\n";

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(cColor[0], cColor[1], cColor[2], cColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	//그리기 부분 구현
	//그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		cColor[0] = 1.0f;
		cColor[1] = 0.0f;
		cColor[2] = 0.0f;
		glutPostRedisplay();
		break;
	case 'g':
		cColor[0] = 0.0f;
		cColor[1] = 1.0f;
		cColor[2] = 0.0f;
		glutPostRedisplay();
		break;
	case 'b':
		cColor[0] = 0.0f;
		cColor[1] = 0.0f;
		cColor[2] = 1.0f;
		glutPostRedisplay();
		break;
	case 'a':
		cColor[0] = dis(engine);
		cColor[1] = dis(engine);
		cColor[2] = dis(engine);
		glutPostRedisplay();
		break;
	case 'w':
		cColor[0] = 1.0f;
		cColor[1] = 1.0f;
		cColor[2] = 1.0f;
		glutPostRedisplay();
		break;
	case 'k':
		cColor[0] = 0.0f;
		cColor[1] = 0.0f;
		cColor[2] = 0.0f;
		glutPostRedisplay();
		break;
	case 't':
		break_p = false;
		glutTimerFunc(100, Timer, 1);
		break;
	case 's':
		break_p = true;
		break;
	case 'q':
		exit(0);
		break;
	}
}

GLvoid Timer(int value)
{
	if (!break_p)
	{
		cColor[0] = dis(engine);
		cColor[1] = dis(engine);
		cColor[2] = dis(engine);
		glutPostRedisplay();
		glutTimerFunc(100, Timer, 1);
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}