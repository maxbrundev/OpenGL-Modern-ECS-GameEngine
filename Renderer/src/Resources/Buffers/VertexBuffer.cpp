#include "renderer_stdafx.h"

#include <Resources/Buffers/VertexBuffer.h>
#include <GL/glew.h>

using namespace Renderer;
using namespace Resources::Buffers;

VertexBuffer::VertexBuffer(): m_rendererID(0)
{}

VertexBuffer::VertexBuffer(const void* p_data, const size_t p_size): m_rendererID(0)
{
	InitBuffer(p_data, p_size);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void VertexBuffer::InitBuffer(const void* p_data, const size_t p_size)
{
	glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, p_size, p_data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}