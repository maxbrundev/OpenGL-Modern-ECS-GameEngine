#pragma once

namespace Renderer
{
	namespace Resources
	{
		namespace Buffers
		{
			class IndexBuffer
			{
			private:
				unsigned m_rendererID;
				unsigned m_count;

			public:
				IndexBuffer();
				IndexBuffer(const unsigned* p_indices, const unsigned p_count);
				~IndexBuffer();

				void InitBuffer(const unsigned* p_indices, const unsigned p_count);
				void Bind() const;
				static void Unbind();

				inline unsigned GetCount() const { return m_count; }
			};

		}
	}
}