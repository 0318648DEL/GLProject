#include <iostream>
#include <random>
#include <chrono>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button,int state,int x, int y);
GLvoid Timer(int value);
GLfloat crColor[3]{ 1.0f,0.0f,1.0f };
GLfloat cbColor[4]{ 1.0f,1.0f,1.0f,1.0f };
std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);

bool break_p = false;

void main(int argc, char** argv)
{
	//윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example2");

	//GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialization\n";

	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(cbColor[0], cbColor[1], cbColor[2], cbColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	//그리기 부분 구현
	glColor3f(crColor[0], crColor[1], crColor[2]);
	glRectf(-0.5f, 0.5f, 0.5f, -0.5f);
	//그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		break_p = true;
		if (x >= 200 && x <= 600 && y >= 150 && y <= 450)
		{
			crColor[0] = dis(engine);
			crColor[1] = dis(engine);
			crColor[2] = dis(engine);
			glutPostRedisplay();
		}
		else
		{
			cbColor[0] = dis(engine);
			cbColor[1] = dis(engine);
			cbColor[2] = dis(engine);
			glutPostRedisplay();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		break_p = false;
		glutTimerFunc(10, Timer, 1);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		break_p = true;
	}
}

GLvoid Timer(int value)
{
	if (!break_p)
	{
		crColor[0] = dis(engine);
		crColor[1] = dis(engine);
		crColor[2] = dis(engine);
		cbColor[0] = dis(engine);
		cbColor[1] = dis(engine);
		cbColor[2] = dis(engine);
		glutPostRedisplay();
		glutTimerFunc(10, Timer, 1);
	}
}