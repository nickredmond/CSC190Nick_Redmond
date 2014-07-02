#include <GlWindow.h>

void GlWindow::initializeGL(){
	glewInit();

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

	glEnableVertexAttribArray(3); // color attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
}
void GlWindow::paintGL(){
	glViewport(0, height() / 2, width() / 2, height() / 2);
	glDrawArrays(GL_TRIANGLES, 0, 3); // notify that we're sending triangle data; start at 0, and aim for 3 vertices (per triangle)
}