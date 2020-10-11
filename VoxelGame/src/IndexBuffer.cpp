#include "IndexBuffer.h"

#include "Renderer.h"

//Takes in array of indicies and the ammount of indicies
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:Count(count)
{
	GLCall(glGenBuffers(1, &RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
//Delete index buffer on destruction
IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
