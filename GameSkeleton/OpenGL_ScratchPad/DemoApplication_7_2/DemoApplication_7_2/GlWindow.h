#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL\glew.h> // always include glew stuff first
#include <QtOpenGL\qglwidget>
#include <Qt\qtimer.h>

class GlWindow : public QGLWidget
{
	Q_OBJECT

	QTimer timer;
public:
	void compileShaders();
	void sendDataToHardware();
	void initializeGL();
	void paintGL();
private slots:
	void update();
};

#endif