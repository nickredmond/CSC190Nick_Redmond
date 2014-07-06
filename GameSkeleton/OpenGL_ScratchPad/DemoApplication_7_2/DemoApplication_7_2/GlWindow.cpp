#include <GlWindow.h>
#include <Qt\qdebug.h>

char* vertexShaderCode =
	"#version 400\r\n"
	""
	"in layout(location=0) vec2 position;"
	"in layout(location=1) vec3 color;"
	""
	"out vec3 deColor;"
	""
	"void main()"
	"{"
	"	gl_Position = vec4(position, 1, 1);"
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

void GlWindow::initializeGL(){
	glewInit();
	sendDataToHardware();
	compileShaders();
}

void GlWindow::sendDataToHardware(){
	float verts[] = // positional data for vertices
	{
		1.0f, -1.0f,
		0.8f, 0.5f, 0.5f,

		0.0f, 1.0f,
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

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);

	glLinkProgram(programID);
	glUseProgram(programID);
}

void GlWindow::paintGL(){
	glViewport(0, height() / 2, width() / 2, height() / 2);
	glDrawArrays(GL_TRIANGLES, 0, 3); // notify that we're sending triangle data; start at 0, and aim for 3 vertices (per triangle)
}