#include "renderer_stdafx.h"

#include <Resources/Buffers/VertexArray.h>
#include <GL/glew.h>

using namespace Renderer;
using namespace Resources::Buffers;

VertexArray::VertexArray() : m_rendererID(0)
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& p_vb, const VertexBufferLayout& p_layout) const
{
	Bind();
	p_vb.Bind();
	std::vector<VertexBufferElement> elements = p_layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < elements.size(); ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, p_layout.GetStride(), reinterpret_cast<const void*>(offset));
		offset += elements[i].count * VertexBufferElement::GetSizeOfType(elements[i].type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}