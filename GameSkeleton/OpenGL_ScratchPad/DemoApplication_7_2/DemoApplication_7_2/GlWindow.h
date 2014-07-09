#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL\glew.h> // always include glew stuff first
#include <QtOpenGL\qglwidget>
#include <Qt\qtimer.h>
#include <Player.h>

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
	void paintPlayer(Player plyr);
	glm::vec2 calculateDrag(glm::vec2 velocity);
};

#endif