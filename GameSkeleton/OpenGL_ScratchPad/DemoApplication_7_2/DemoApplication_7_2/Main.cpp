#include <QtGui\qapplication.h>
#include <QtGui\qwidget.h>

#include <GlWindow.h>

void main(int argumentCount, char* argumentVector[])
{
	QApplication app(argumentCount, argumentVector);

	GlWindow window;
	window.show();

	app.exec();
}