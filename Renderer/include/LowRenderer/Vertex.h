#pragma once

#include "renderer_stdafx.h"

namespace Renderer
{
	namespace LowRenderer
	{
		class Vertex
		{
		private:
			Core::Maths::vec3f m_position;
			Core::Maths::vec3f m_normal;
			Core::Maths::vec2f m_textureCoordinate;

		public:
			explicit Vertex(const Core::Maths::vec3f& p_position = Core::Maths::vec3f(), const Core::Maths::vec3f& p_normal = Core::Maths::vec3f(), const Core::Maths::vec2f& p_textureCoordinate = Core::Maths::vec2f(0, 0));
			Vertex(const Core::Maths::vec3f& p_position, const Core::Maths::vec2f& p_textureCoordinate);

			Core::Maths::vec3f& Position();
			const Core::Maths::vec3f& Position() const;
			Core::Maths::vec3f& Normal();
			const Core::Maths::vec3f& Normal() const;
			Core::Maths::vec2f& TextureCoordinate();
			const Core::Maths::vec2f& TextureCoordinate() const;
		};
	}
}