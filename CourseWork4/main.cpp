// stb
#include "./include/Vendor/stb_image/stb_image.h"

#include "./include/CusMath.hpp"
#include "./include/GLUTCallback.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 768

#define SUB_WINDOW_WIDTH 350
#define SUB_WINDOW_HEIGHT 450

void timerProc(int id);

vector<GLuint> GWindows;

int main(int argc,char *argv[])
{
    // 初始化glut
	glutInit(&argc,argv);
	
    // 显示模式初始化(彩色RGBA+双缓存)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
    // 窗口窗口位置
    float x = (SCREEN_WIDTH - WINDOW_WIDTH) * 0.5f;
    float y = (SCREEN_HEIGHT - WINDOW_HEIGHT) * 0.5f;
    glutInitWindowPosition(x, y);
	
    // 窗口大小
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	
    // 主窗口名称
    GLuint mainWindow = glutCreateWindow("OpenGL Window");
    GWindows.emplace_back(mainWindow);

    GLUTCallback::setWinWidth(WINDOW_WIDTH);
    GLUTCallback::setWinHeight(WINDOW_HEIGHT);

    GLUTCallback::setSubWinWidth(SUB_WINDOW_WIDTH);
    GLUTCallback::setSubWinHeight(SUB_WINDOW_HEIGHT);

    // 自定义函数绘图
    glutDisplayFunc(&GLUTCallback::myDisplay);
    glutReshapeFunc(&GLUTCallback::myReshape);
    glutKeyboardFunc(&GLUTCallback::myKeyboardFunc);
    glutMouseFunc(&GLUTCallback::myMouseFunc);

    // 子窗口的左上角起点是相对主窗口而言
    GLuint subWindow = glutCreateSubWindow(mainWindow, WINDOW_WIDTH - SUB_WINDOW_WIDTH, 
        (WINDOW_HEIGHT - SUB_WINDOW_HEIGHT) * 0.5f, SUB_WINDOW_WIDTH, SUB_WINDOW_HEIGHT
    );
    GWindows.emplace_back(subWindow);

	glutDisplayFunc(&GLUTCallback::mySubWinDisplay);
    glutReshapeFunc(&GLUTCallback::myReshape);
    glutKeyboardFunc(&GLUTCallback::mySubKeyboardFunc);
    glutMouseFunc(&GLUTCallback::mySubMouseFunc);

    glutTimerFunc(100, timerProc, 1);

    // 消息循环相应
    glutMainLoop();
	
    return 0;
}

void timerProc(int id)
{
    for (auto w : GWindows)
    {
        glutSetWindow(w);
        glutPostRedisplay();
    }
    
    // 需要在函数中再调用一次，才能保证循环
    glutTimerFunc(10, timerProc, 1);
}