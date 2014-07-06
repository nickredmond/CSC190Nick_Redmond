#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL\glew.h> // always include glew stuff first
#include <QtOpenGL\qglwidget>

class GlWindow : public QGLWidget
{
public:
	void compileShaders();
	void sendDataToHardware();
	void initializeGL();
	void paintGL();
};

#endif