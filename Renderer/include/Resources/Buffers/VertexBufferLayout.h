#pragma once

#include <vector>

#include <GL/glew.h>

#include "Debug/Assertion.h"

namespace Renderer
{
	namespace Resources
	{
		namespace Buffers
		{
			struct VertexBufferElement
			{
				unsigned int type;
				unsigned int count;
				unsigned char normalized;

				static unsigned int GetSizeOfType(const unsigned int p_type)
				{
					switch (p_type)
					{
					case GL_FLOAT:			return 4;
					case GL_UNSIGNED_INT:	return 4;
					case GL_UNSIGNED_BYTE:	return 1;
					}
					ASSERT(false, "Unhandled type");
				}
			};

			class VertexBufferLayout
			{
			private:
				std::vector<VertexBufferElement> m_elements;
				unsigned int m_stride;

			public:
				VertexBufferLayout() : m_stride(0) {}
				~VertexBufferLayout() {}
				template<typename T>
				void Push(unsigned int count) {}

				template<>
				void Push<float>(const unsigned int count)
				{
					m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
					m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
				}

				template<>
				void Push<unsigned int>(const unsigned int count)
				{
					m_elements.push_back({ GL_UNSIGNED_INT,count, GL_FALSE });
					m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
				}

				template<>
				void Push<unsigned char>(const unsigned int count)
				{
					m_elements.push_back({ GL_UNSIGNED_BYTE,count, GL_TRUE });
					m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
				}

				inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
				inline unsigned int GetStride() const { return m_stride; }
			};
		}
	}
}