#include <GlWindow.h>
#include <Qt\qdebug.h>
#include <glm\glm.hpp>
#include <Matrix3.h>

char* vertexShaderCode =
	"#version 400\r\n"
	""
	"in layout(location=0) vec2 position;"
	"in layout(location=1) vec3 color;"
	""
	"uniform mat4 transform;"
	""
	"out vec3 deColor;"
	""
	"void main()"
	"{"
	"	gl_Position = transform * vec4(position, 1, 1);"
	"	deColor = color;"
	"}"
	"";

char* fragmentShaderCode =
	"#version 400\r\n"
	""
	"in vec3 deColor;"
	"out vec3 theFinalColor;"
	""
	"void main()"
	"{"
	"	theFinalColor = deColor;"
	"}";

GLuint programID;
glm::vec2 truePosition;
glm::vec2 velocity;
float angle;

const float ACCELERATION = 0.0006f;
const float OMEGA = 0.1f;
const float TRI_SCALE = 0.1f;

void GlWindow::initializeGL(){
	glewInit();
	sendDataToHardware();
	compileShaders();
}

void GlWindow::sendDataToHardware(){
	float verts[] = // positional data for vertices
	{
		1.0f, 0.0f,
		0.8f, 0.5f, 0.5f,

		-1.0f, 1.0f,
		1.0f, 0.1f, 0.3f,

		-1.0f, -1.0f,
		1.0f, 0.9f, 0.7f
	};

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sending a memory buffer for the vertex array (6 floats, size of 24 bytes)
	glEnableVertexAttribArray(0); // vertices have attributes, including position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0); // describing the data being sent to OpenGL (we're describing positions made of 2 floats)

	glEnableVertexAttribArray(1); // color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(0);
}

void GlWindow::compileShaders(){
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	adapter[0] = fragmentShaderCode;
	glShaderSource(fragShaderID, 1, adapter, 0);
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);

	glCompileShader(fragShaderID);
	glCompileShader(vertexShaderID);

	GLint compileStatus;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileStatus);

	if(compileStatus != GL_TRUE){
		GLint logLength;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer = new char[logLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(vertexShaderID, logLength, &bitBucket, buffer);
		qDebug() << buffer;
		delete [] buffer;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);

	glLinkProgram(programID);
	glUseProgram(programID);
}

void GlWindow::paintGL(){
	glm::mat3 transform = Matrix3::Translation(glm::vec3(truePosition, 1)) * Matrix3::Scale(TRI_SCALE) * Matrix3::Rotation(angle) * Matrix3::Translation(glm::vec3(truePosition, 1));
	glm::mat4 finalTransform = glm::mat4(transform);

	GLint transformLocation = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &finalTransform[0][0]);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3); // notify that we're sending triangle data; start at 0, and aim for 3 vertices (per triangle)
}

void GlWindow::update(){
	if (GetAsyncKeyState(VK_UP)){
		velocity += glm::vec2(cos(angle), sin(angle)) * ACCELERATION;
	}
	if (GetAsyncKeyState(VK_DOWN)){
		velocity -= glm::vec2(cos(angle), sin(angle)) * ACCELERATION;
	}
	if (GetAsyncKeyState(VK_LEFT)){
		angle += OMEGA;
	}
	if (GetAsyncKeyState(VK_RIGHT)){
		angle -= OMEGA;
	}

	truePosition += velocity;

	repaint();
}