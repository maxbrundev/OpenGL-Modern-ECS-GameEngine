#pragma once

#include <Resources/Buffers/VertexBuffer.h>
#include <Resources/Buffers/VertexBufferLayout.h>

namespace Renderer
{
	namespace Resources
	{
		namespace Buffers
		{
			class VertexArray
			{
			private:
				unsigned int m_rendererID;

			public:
				VertexArray();
				~VertexArray();

				void AddBuffer(const VertexBuffer& p_vb, const VertexBufferLayout& p_layout) const;

				void Bind() const;
				static void Unbind();
			};
		}
	}
}