#pragma once

#include "renderer_stdafx.h"

namespace Renderer
{
	namespace LowRenderer
	{
		class Color
		{
		private:
			union
			{
				unsigned char m_rgba[4];
				struct
				{
					unsigned char m_r;
					unsigned char m_g;
					unsigned char m_b;
					unsigned char m_a;
				};
			};

		public:
			Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
			Color(float r, float g, float b, float a = 1.0f);
			explicit Color(const Core::Maths::vec3f& rgb, float a = 1.0f);
			explicit Color(const Core::Maths::vec4f& rgba);
			explicit Color(const Core::Maths::vec3<unsigned char>& rgb, unsigned char a = 1.0f);
			explicit Color(const Core::Maths::vec4<unsigned char>& rgba);
			~Color() = default;
		};
	}
}