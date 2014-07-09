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
	"uniform vec3 dominatingColor;"
	""
	"out vec3 deColor;"
	""
	"void main()"
	"{"
	"	gl_Position = transform * vec4(position, 1, 1);"
	"	deColor = dominatingColor;"
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
Player player1, player2;

const float ACCELERATION = 0.0006f;
const float OMEGA = 0.1f;
const float TRI_SCALE = 0.1f;
const float PI = 3.14159265f;
const float DRAG_COEFF = 0.02f;

void GlWindow::initializeGL(){
	player1.color = glm::vec3(1, 0, 0);
	player2.color = glm::vec3(0, 0, 1);
	player1.position = glm::vec2(-0.5, 0);
	player2.position = glm::vec2(0.5, 0);
	player2.angle = PI;

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

	GLushort indices[] = {0, 1, 2};
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

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
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	paintPlayer(player1);
	paintPlayer(player2);
}

void GlWindow::paintPlayer(Player plyr){
	glm::mat3 transform = Matrix3::Translation(glm::vec3(plyr.position, 1)) * Matrix3::Scale(TRI_SCALE) * 
		Matrix3::Rotation(plyr.angle) * Matrix3::Translation(glm::vec3(plyr.position, 1));
	glm::mat4 finalTransform = glm::mat4(transform);

	GLint transformLocation = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &finalTransform[0][0]);

	GLint colorLocation = glGetUniformLocation(programID, "dominatingColor");
	glUniform3f(colorLocation, plyr.color.r, plyr.color.g, plyr.color.b);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
}

glm::vec2 GlWindow::calculateDrag(glm::vec2 velocity){
	glm::vec2 drag = glm::vec2(-velocity.x, -velocity.y) * DRAG_COEFF;

	return drag;
}

void GlWindow::update(){
	if (GetAsyncKeyState('W')){
		player1.velocity += glm::vec2(cos(player1.angle), sin(player1.angle)) * ACCELERATION;
	}
	else if (GetAsyncKeyState('S')){
		player1.velocity -= glm::vec2(cos(player1.angle), sin(player1.angle)) * ACCELERATION;
	}
	else if (player1.velocity.length() > 0.0001f){
		player1.velocity += calculateDrag(player1.velocity);
	}

	if (GetAsyncKeyState('A')){
		player1.angle += OMEGA;
	}
	if (GetAsyncKeyState('D')){
		player1.angle -= OMEGA;
	}

	if (GetAsyncKeyState(VK_UP)){
		player2.velocity += glm::vec2(cos(player2.angle), sin(player2.angle)) * ACCELERATION;
	}
	else if (GetAsyncKeyState(VK_DOWN)){
		player2.velocity -= glm::vec2(cos(player2.angle), sin(player2.angle)) * ACCELERATION;
	}
	else if (player2.velocity.length() > 0.0001f){
		player2.velocity += calculateDrag(player2.velocity);
	}

	if (GetAsyncKeyState(VK_LEFT)){
		player2.angle += OMEGA;
	}
	if (GetAsyncKeyState(VK_RIGHT)){
		player2.angle -= OMEGA;
	}

	player1.position += player1.velocity;
	player2.position += player2.velocity;

	repaint();
}