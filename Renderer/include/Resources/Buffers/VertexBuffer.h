#pragma once

namespace Renderer
{
	namespace Resources
	{
		namespace Buffers
		{
			class VertexBuffer
			{
			private:
				unsigned m_rendererID;

			public:
				VertexBuffer();
				VertexBuffer(const void* p_data, const size_t p_size);
				~VertexBuffer();

				void InitBuffer(const void* p_data, const size_t p_size);
				void Bind() const;
				static void Unbind();
			};
		}
	}
}
