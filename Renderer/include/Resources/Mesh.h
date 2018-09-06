#pragma once

#include "renderer_stdafx.h"

#include "LowRenderer/Vertex.h"

#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"

namespace Renderer
{
	namespace Resources
	{
		namespace Buffers
		{
			class VertexBufferLayout;
			class VertexArray;
		}

		class Mesh
		{
		private:
			static const std::regex VERTEX_PATTERN;
			static const std::regex NORMAL_PATTERN;
			static const std::regex UV_PATTERN;
			static const std::regex FACE_VERTEX_PATTERN;
			static const std::regex FACE_VERTEX_UV_PATTERN;
			static const std::regex FACE_VERTEX_UV_NORMAL_PATTERN;
			static const std::regex FACE_VERTEX_NORMAL_PATTERN;
			static const std::regex OBJECT_PATTERN;
			static const std::regex SMOOTHING_PATTERN;
			static const std::regex MTL_LIB_PATTERN;
			static const std::regex MTL_USE_PATTERN;

			std::vector<LowRenderer::Vertex>	m_loadedVertices;
			std::vector<unsigned int>			m_loadedIndices;

			//		std::vector<Material>				m_loadedMaterials;

			Buffers::VertexBuffer	m_vb;
			Buffers::IndexBuffer	m_ib;

		public:
			explicit Mesh(const std::string&& p_filePath);
			~Mesh() = default;

			void InitBuffers(Buffers::VertexArray& p_va, Buffers::VertexBufferLayout& p_layout);
			bool ParseOBJ(const std::string& p_filePath);

			const Buffers::IndexBuffer&		GetIndexBuffer() const;
			const Buffers::VertexBuffer&	GetVertexBuffer() const;

		private:
			void GenFace(std::vector<LowRenderer::Vertex>& o_faceVertices,
				std::vector<LowRenderer::Vertex>& o_vertices,
				const std::vector<Core::Maths::vec3f>& p_positions,
				const std::vector<Core::Maths::vec2f>& p_tCoords,
				const std::vector<Core::Maths::vec3f>& p_normals,
				std::string&& p_line);

			void TriangulateFace(std::vector<unsigned int>& o_indices,
				const std::vector<LowRenderer::Vertex>& p_verts) const;

			bool IsInsideTriangle(const Core::Maths::vec3f& p_point,
				const Core::Maths::vec3f& p_triangle1,
				const Core::Maths::vec3f& p_triangle2,
				const Core::Maths::vec3f& p_triangle3) const;

			unsigned GetIndex(const std::string& p_index) const;

			template <class T>
			const T& GetElement(const std::vector<T>& p_elements, const std::string& p_index);
		};
	}
}