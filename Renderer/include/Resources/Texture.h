#pragma once

#include "renderer_stdafx.h"

namespace Renderer
{
	namespace Resources
	{
		class Texture
		{
		private:
			unsigned int m_rendererID;
			std::string m_filePath;
			unsigned char* m_buffer;
			int m_width, m_height, m_BPP;

		public:
			explicit Texture(const std::string&& p_filepath);
			~Texture();

			void Bind(const unsigned int p_slot = 0) const;
			static void Unbind();
		};
	}
}