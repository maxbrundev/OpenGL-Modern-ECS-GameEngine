#include "renderer_stdafx.h"

#include <LowRenderer/Vertex.h>

using namespace Renderer;
using namespace LowRenderer;

Vertex::Vertex(const Core::Maths::vec3f& p_position, const Core::Maths::vec3f& p_normal,
	const Core::Maths::vec2f& p_textureCoordinate): m_position(p_position),m_normal(p_normal),m_textureCoordinate(p_textureCoordinate)
{}

Vertex::Vertex(const Core::Maths::vec3f& p_position, const Core::Maths::vec2f& p_textureCoordinate):
	m_position(p_position), m_textureCoordinate(p_textureCoordinate)
{}

Core::Maths::vec3f& Vertex::Position()
{
	return m_position;
}

const Core::Maths::vec3f& Vertex::Position() const
{
	return m_position;
}

Core::Maths::vec3f& Vertex::Normal()
{
	return m_normal;
}

const Core::Maths::vec3f& Vertex::Normal() const
{
	return m_normal;
}

Core::Maths::vec2f& Vertex::TextureCoordinate()
{
	return m_textureCoordinate;
}

const Core::Maths::vec2f& Vertex::TextureCoordinate() const
{
	return m_textureCoordinate;
}