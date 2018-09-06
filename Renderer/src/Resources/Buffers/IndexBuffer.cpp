#include "renderer_stdafx.h"

#include <Resources/Buffers/IndexBuffer.h>
#include <GL/glew.h>

using namespace Renderer;
using namespace Resources::Buffers;

IndexBuffer::IndexBuffer(): m_rendererID(0), m_count(0)
{}

IndexBuffer::IndexBuffer(const unsigned int* p_indices, const unsigned int p_count): m_rendererID(0), m_count(p_count)
{
	InitBuffer(p_indices, p_count);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void IndexBuffer::InitBuffer(const unsigned int* p_indices, const unsigned int p_count)
{
	m_count = p_count;
	glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_count * sizeof(unsigned int), p_indices, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}