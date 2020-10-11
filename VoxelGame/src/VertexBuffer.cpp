#include "VertexBuffer.h"

#include "Renderer.h"

//Takes in array of verticies and the total size in bytes
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
//Delete vertex buffer on destruction
VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
