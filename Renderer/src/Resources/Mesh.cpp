#include "renderer_stdafx.h"

#include "Resources/Mesh.h"

#include "Resources/Buffers/VertexArray.h"

#include "Debug/Log.h"
#include "Debug/Assertion.h"

#include "Utils/trim.h"

using namespace Renderer;
using namespace Core::Maths;
using namespace LowRenderer;
using namespace Resources;
using namespace Buffers;

const std::regex Mesh::VERTEX_PATTERN("^v\\s+([\\d|\\.|\\+|\\-|e|E]+)\\s+([\\d|\\.|\\+|\\-|e|E]+)\\s+([\\d|\\.|\\+|\\-|e|E]+)");
const std::regex Mesh::NORMAL_PATTERN("^vn\\s+([\\d|\\.|\\+|\\-|e|E]+)\\s+([\\d|\\.|\\+|\\-|e|E]+)\\s+([\\d|\\.|\\+|\\-|e|E]+)");
const std::regex Mesh::UV_PATTERN("^vt\\s+([\\d|\\.|\\+|\\-|e|E]+)\\s+([\\d|\\.|\\+|\\-|e|E]+)");
const std::regex Mesh::FACE_VERTEX_PATTERN("^f\\s+(-?\\d+)\\s+(-?\\d+)\\s+(-?\\d+)(?:\\s+(-?\\d+))?");
const std::regex Mesh::FACE_VERTEX_UV_PATTERN("^f\\s+(-?\\d+)\\/(-?\\d+)\\s+(-?\\d+)\\/(-?\\d+)\\s+(-?\\d+)\\/(-?\\d+)(?:\\s+(-?\\d+)\\/(-?\\d+))?");
const std::regex Mesh::FACE_VERTEX_UV_NORMAL_PATTERN("^f\\s+(-?\\d+)\\/(-?\\d+)\\/(-?\\d+)\\s+(-?\\d+)\\/(-?\\d+)\\/(-?\\d+)\\s+(-?\\d+)\\/(-?\\d+)\\/(-?\\d+)(?:\\s+(-?\\d+)\\/(-?\\d+)\\/(-?\\d+))?");
const std::regex Mesh::FACE_VERTEX_NORMAL_PATTERN("^f\\s+(-?\\d+)\\/\\/(-?\\d+)\\s+(-?\\d+)\\/\\/(-?\\d+)\\s+(-?\\d+)\\/\\/(-?\\d+)(?:\\s+(-?\\d+)\\/\\/(-?\\d+))?");
const std::regex Mesh::OBJECT_PATTERN("^[og]\\s*(.+)?");
const std::regex Mesh::SMOOTHING_PATTERN("^s\\s+(\\d+|on|off)");
const std::regex Mesh::MTL_LIB_PATTERN("^mtllib ");
const std::regex Mesh::MTL_USE_PATTERN("^usemtl ");

Mesh::Mesh(const std::string&& p_filePath)
{
	ParseOBJ(p_filePath);
}

void Mesh::InitBuffers(VertexArray& p_va,VertexBufferLayout& p_layout)
{
	m_vb.InitBuffer(m_loadedVertices.data(), m_loadedVertices.size() * sizeof(Vertex));
	p_layout.Push<float>(3);
	p_layout.Push<float>(3);
	p_layout.Push<float>(2);
	p_va.AddBuffer(m_vb, p_layout);

	m_ib.InitBuffer(m_loadedIndices.data(), static_cast<unsigned int>(m_loadedIndices.size()));
}

bool Mesh::ParseOBJ(const std::string& p_filePath)
{
	try
	{
		if (p_filePath.substr(p_filePath.size() - 4, 4) == ".obj")
		{
			std::ifstream	objFile(p_filePath);
			std::string		line;
			if (objFile.is_open())
			{
				m_loadedVertices.clear();
				m_loadedIndices.clear();

				std::vector<vec3f>		positions;
				std::vector<vec2f>		textureCoords;
				std::vector<vec3f>		normals;
				std::vector<unsigned>	indices;
				std::vector<Vertex>		vertices;

				while (getline(objFile, line))
				{
					Utils::trim(line);
					const size_t lineLength = line.length();

					if (lineLength == 0) 
						continue;

					if (line[0] == 'v')
					{
						std::smatch result;
						if (line[1] == ' ')
						{
							std::regex_search(line, result, VERTEX_PATTERN);
							// 0                  1      2      3
							// ["v 1.0 2.0 3.0", "1.0", "2.0", "3.0"]
							if (!result.empty())
								positions.emplace_back(std::stof(result[1]), std::stof(result[2]), std::stof(result[3]));
						}
						else if (line[1] == 'n')
						{
							std::regex_search(line, result, NORMAL_PATTERN);
							// 0                   1      2      3
							// ["vn 1.0 2.0 3.0", "1.0", "2.0", "3.0"]

							if (!result.empty())
								normals.emplace_back(std::stof(result[1]), std::stof(result[2]), std::stof(result[3]));
						}
						else if (line[1] == 't')
						{
							std::regex_search(line, result, UV_PATTERN);
							// 0               1      2
							// ["vt 0.1 0.2", "0.1", "0.2"]

							if (!result.empty())
							{
								textureCoords.emplace_back(std::stof(result[1]), std::stof(result[2]));
							}
						}
						else
						{
							ASSERT(0, "Unexpected vertex/normal/uv line: " + line);
						}
					}
					else if (line[0] == 'f')
					{
						std::vector<Vertex> currFaceVertices;
						std::vector<unsigned> currFaceIndices;
						GenFace(currFaceVertices, vertices, positions, textureCoords, normals, std::move(line));
						TriangulateFace(currFaceIndices, currFaceVertices);

						for (int i = 0; i < int(currFaceIndices.size()); i++)
						{
							unsigned int indnum = static_cast<unsigned int>((vertices.size()) - currFaceVertices.size()) + currFaceIndices[i];
							indices.push_back(indnum);

							indnum = static_cast<unsigned int>((m_loadedVertices.size()) - currFaceVertices.size()) + currFaceIndices[i];
							m_loadedIndices.push_back(indnum);
						}
					}
				}
				objFile.close();
				if (!m_loadedVertices.empty() || !m_loadedIndices.empty())
				{
					return true;
				}
				return false;
			}
			std::string logmsg("Unable to open file" + p_filePath);
			LOG(Error(), logmsg);
			return false;
		}
		std::string logmsg("File " + p_filePath + "is not a .obj");
		LOG(Error(), logmsg);
		return false;
	}
	catch (...)
	{
		return false;
	}
}

const Buffers::IndexBuffer& Mesh::GetIndexBuffer() const
{
	return m_ib;
}

const Buffers::VertexBuffer& Mesh::GetVertexBuffer() const
{
	return m_vb;
}

void Mesh::GenFace(std::vector<Vertex>& o_faceVertices,
					std::vector<Vertex>& o_vertices,
                    const std::vector<vec3f>& p_positions,
                    const std::vector<vec2f>& p_tCoords,
                    const std::vector<vec3f>& p_normals,
                    std::string&& p_line)
{
	std::smatch result;

	std::regex_search(p_line, result, FACE_VERTEX_UV_NORMAL_PATTERN);
	// f vertex/uv/normal vertex/uv/normal vertex/uv/normal
	// 0                        1    2    3    4    5    6    7    8    9
	// ["f 1/1/1 2/2/2 3/3/3", "1", "1", "1", "2", "2", "2", "3", "3", "3"]
	if (!result.empty())
	{
		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(1)),
			GetElement(p_normals, result.str(3)),
			GetElement(p_tCoords, result.str(2)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(4)),
			GetElement(p_normals, result.str(6)),
			GetElement(p_tCoords, result.str(5)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(7)),
			GetElement(p_normals, result.str(9)),
			GetElement(p_tCoords, result.str(8)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		if (result[10].matched)
		{
			o_faceVertices.emplace_back(
				GetElement(p_positions, result.str(10)),
				GetElement(p_normals, result.str(12)),
				GetElement(p_tCoords, result.str(11)));

			o_vertices.push_back(o_faceVertices.back());
			m_loadedVertices.push_back(o_faceVertices.back());
		}
		return;
	}

	std::regex_search(p_line, result, FACE_VERTEX_UV_PATTERN);
	// f vertex/uv vertex/uv vertex/uv
	// 0                  1    2    3    4    5    6
	// ["f 1/1 2/2 3/3", "1", "1", "2", "2", "3", "3"]
	if (!result.empty())
	{
		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(1)),
			GetElement(p_tCoords, result.str(2)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(3)),
			GetElement(p_tCoords, result.str(4)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(5)),
			GetElement(p_tCoords, result.str(6)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		if (result[7].matched)
		{
			o_faceVertices.emplace_back(
				GetElement(p_positions, result.str(7)),
				GetElement(p_tCoords, result.str(8)));

			o_vertices.push_back(o_faceVertices.back());
			m_loadedVertices.push_back(o_faceVertices.back());
		}
		return;
	}

	std::regex_search(p_line, result, FACE_VERTEX_NORMAL_PATTERN);
	// f vertex//normal vertex//normal vertex//normal
	// 0                     1    2    3    4    5    6
	// ["f 1//1 2//2 3//3", "1", "1", "2", "2", "3", "3"]
	if (!result.empty())
	{
		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(1)),
			GetElement(p_normals, result.str(2)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(3)),
			GetElement(p_normals, result.str(4)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(5)),
			GetElement(p_normals, result.str(6)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		if (result[7].matched)
		{
			o_faceVertices.emplace_back(
				GetElement(p_positions, result.str(7)),
				GetElement(p_normals, result.str(8)));

			o_vertices.push_back(o_faceVertices.back());
			m_loadedVertices.push_back(o_faceVertices.back());
		}
		return;
	}

	std::regex_search(p_line, result, FACE_VERTEX_PATTERN);
	// f vertex vertex vertex
	// 0            1    2    3
	// ["f 1 2 3", "1", "2", "3"]
	if (!result.empty())
	{
		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(1)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(2)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		o_faceVertices.emplace_back(
			GetElement(p_positions, result.str(3)));

		o_vertices.push_back(o_faceVertices.back());
		m_loadedVertices.push_back(o_faceVertices.back());

		if (result[4].matched)
		{
			o_faceVertices.emplace_back(
				GetElement(p_positions, result.str(4)));

			o_vertices.push_back(o_faceVertices.back());
			m_loadedVertices.push_back(o_faceVertices.back());
		}
	}
}

void Mesh::TriangulateFace(std::vector<unsigned>& o_indices, 
							const std::vector<Vertex>& p_verts) const
{
	if (p_verts.size() < 3)
	{
		return;
	}
	// If it is a triangle no need to calculate it
	if (p_verts.size() == 3)
	{
		o_indices.push_back(0);
		o_indices.push_back(1);
		o_indices.push_back(2);
		return;
	}

	// Create a list of vertices
	std::vector<Vertex> tVerts = p_verts;

	while (true)
	{
		// For every vertex
		for (int i = 0; i < int(tVerts.size()); i++)
		{
			Vertex pPrev;
			if (i == 0)
			{
				pPrev = tVerts[tVerts.size() - 1];
			}
			else
			{
				pPrev = tVerts[i - 1];
			}

			Vertex pCur = tVerts[i];

			Vertex pNext;
			if (i == tVerts.size() - 1)
			{
				pNext = tVerts[0];
			}
			else
			{
				pNext = tVerts[i + 1];
			}

			// if last triangle
			if (tVerts.size() == 3)
			{
				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].Position() == pCur.Position())
						o_indices.push_back(j);
					if (tVerts[j].Position() == pPrev.Position())
						o_indices.push_back(j);
					if (tVerts[j].Position() == pNext.Position())
						o_indices.push_back(j);
				}

				tVerts.clear();
				break;
			}

			if (tVerts.size() == 4)
			{
				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].Position() == pCur.Position())
						o_indices.push_back(j);
					if (tVerts[j].Position() == pPrev.Position())
						o_indices.push_back(j);
					if (tVerts[j].Position() == pNext.Position())
						o_indices.push_back(j);
				}

				vec3f lastVertPos;
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].Position() != pCur.Position()
						&& tVerts[j].Position() != pPrev.Position()
						&& tVerts[j].Position() != pNext.Position())
					{
						lastVertPos = tVerts[j].Position();
						break;
					}
				}

				// Create a triangle from pPrev, pNext, lastPos
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].Position() == pPrev.Position())
						o_indices.push_back(j);
					if (tVerts[j].Position() == pNext.Position())
						o_indices.push_back(j);
					if (tVerts[j].Position() == lastVertPos)
						o_indices.push_back(j);
				}

				tVerts.clear();
				break;
			}

			// If Vertex is not an interior vertex
			const float angle = vec3f::ToDegree(vec3f::AngleBetween(pPrev.Position() - pCur.Position(), pNext.Position() - pCur.Position()));
			if (angle <= 0 && angle >= 180)
				continue;

			// If any vertices are within this triangle
			bool inTri = false;
			for (int j = 0; j < int(p_verts.size()); j++)
			{
				if (IsInsideTriangle(tVerts[j].Position(), pPrev.Position(), pCur.Position(), pNext.Position())
					&& tVerts[j].Position() != pPrev.Position()
					&& tVerts[j].Position() != pCur.Position()
					&& tVerts[j].Position() != pNext.Position())
				{
					inTri = true;
					break;
				}
			}
			if (inTri)
				continue;

			// Create a triangle from pCur, pPrev, pNext
			for (int j = 0; j < int(p_verts.size()); j++)
			{
				if (tVerts[j].Position() == pCur.Position())
					o_indices.push_back(j);
				if (tVerts[j].Position() == pPrev.Position())
					o_indices.push_back(j);
				if (tVerts[j].Position() == pNext.Position())
					o_indices.push_back(j);
			}

			// Delete pCur from the list
			for (int j = 0; j < int(tVerts.size()); j++)
			{
				if (tVerts[j].Position() == pCur.Position())
				{
					tVerts.erase(tVerts.begin() + j);
					break;
				}
			}

			// reset i to the start
			// -1 since loop will add 1 to it
			i = -1;

		}

		// if no triangles were created
		if (o_indices.size() == 0)
			break;

		// if no more vertices
		if (tVerts.size() == 0)
			break;
	}
}

bool Mesh::IsInsideTriangle(const vec3f& p_point, const vec3f& p_triangle1, const vec3f& p_triangle2, const vec3f& p_triangle3) const
{
	// Starting vars
	const vec3f u = p_triangle2 - p_triangle1;
	const vec3f v = p_triangle3 - p_triangle1;
	const vec3f w = p_point - p_triangle1;
	const vec3f n = vec3f::CrossProduct(u, v);

	const float y = (vec3f::DotProduct(vec3f::CrossProduct(u, w), n) / vec3f::DotProduct(n, n));
	const float b = (vec3f::DotProduct(vec3f::CrossProduct(u, w), n) / vec3f::DotProduct(n, n));
	const float a = 1 - y - b;

	if (a >= 0 && a <= 1
		&& b >= 0 && b <= 1
		&& y >= 0 && y <= 1)
	{
		return true;
	}
	return false;
}

unsigned Mesh::GetIndex(const std::string& p_index) const
{
	return std::stoul(p_index) - 1;
}


template <class T>
const T& Mesh::GetElement(const std::vector<T>& p_elements, const std::string& p_index)
{
	return p_elements[GetIndex(p_index)];
}