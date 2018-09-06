#include "renderer_stdafx.h"

#include <Resources/Shader.h>

#include "Debug/Log.h"

using namespace Renderer;
using namespace Resources;

std::string Shader::vertexShaderStart("#shader vertex");
std::string Shader::fragmentShaderStart("#shader fragment");

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

Shader::Shader(const std::string&& p_filepath): m_rendererID(0)
{
	const ShaderProgramSource sources = Parse(std::move(p_filepath));
	Compile(sources);
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
}


ShaderProgramSource Shader::Parse(const std::string&& p_filepath)
{
	std::ifstream shaderFile;
	shaderFile.open(p_filepath);
	std::string shader;
	ShaderProgramSource sources;

	if (shaderFile.is_open())
	{
		shaderFile.seekg(0, std::ios::end);
		shader.reserve(shaderFile.tellg());
		shaderFile.seekg(0, std::ios::beg);

		shader.assign(std::istreambuf_iterator<char>(shaderFile),
			std::istreambuf_iterator<char>());
	}
	else
	{
		std::cerr << "Error reading shader file : " << p_filepath << "\n";
		shaderFile.close();
		return {};
	}
	shaderFile.close();

	const GLsizei vtStartIndex = static_cast<GLsizei>(shader.find(vertexShaderStart) + vertexShaderStart.size());
	const GLsizei fragStartIndex = static_cast<GLsizei>(shader.find(fragmentShaderStart));

	if (shader.size() != 0 && vtStartIndex != 0 && fragStartIndex != 0)
	{
		sources.VertexSource = shader.substr(vtStartIndex, fragStartIndex - vtStartIndex);
		sources.FragmentSource = shader.substr(fragStartIndex + fragmentShaderStart.size());
	}
	else
	{
		std::cerr << "Error finding shaders in file : " << p_filepath << "\n";
	}

	return sources;
}

void Shader::Compile(const ShaderProgramSource& p_sources)
{
	if (p_sources.VertexSource.size() > 0 && p_sources.FragmentSource.size() > 0)
	{
		//Because GLchar can't cast from std::string
		const char* vtShaderChar = p_sources.VertexSource.c_str();
		const char* fragShaderChar = p_sources.FragmentSource.c_str();

		const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vtShaderChar, nullptr);
		glCompileShader(vertexShader);

		CatchCompilerError(GL_COMPILE_STATUS, vertexShader);


		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragShaderChar, nullptr);
		glCompileShader(fragmentShader);

		CatchCompilerError(GL_COMPILE_STATUS, fragmentShader);

		m_rendererID = glCreateProgram();
		glAttachShader(m_rendererID, vertexShader);
		glAttachShader(m_rendererID, fragmentShader);
		glLinkProgram(m_rendererID);
		CatchCompilerError(GL_LINK_STATUS, m_rendererID);


		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}

void Shader::Bind() const
{
	glUseProgram(m_rendererID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::CatchCompilerError(const GLenum p_compilerAction, const GLuint p_compiledShader)
{
	int success;
	int length = 0;
	char* err = nullptr;

	glGetShaderiv(p_compiledShader, p_compilerAction, &success);
	if (success == GL_FALSE)
	{
		glGetShaderiv(p_compiledShader, GL_INFO_LOG_LENGTH, &length);
		err = static_cast<char*>(alloca(length));
		glGetShaderInfoLog(p_compiledShader, length, nullptr, err);
		LOG(Error(), "Shader " << ((p_compilerAction == GL_COMPILE_STATUS) ? "Compilation" : "Linking") <<
			" failed : " << err);
	}
}

int Shader::GetUniformLocation(const std::string&& p_name)
{
	if (m_uniformLocationCache.find(p_name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[p_name];

	const int location = glGetUniformLocation(m_rendererID, p_name.c_str());
	if (location == -1)
		LOG(Warning(), "Uniform " + p_name + " doesn't exist");

	m_uniformLocationCache[p_name] = location;

	return location;
}

/* Uniforms */
void Shader::SetUniform(const std::string&& p_name, const int p_value)
{
	glUniform1i(GetUniformLocation(std::move(p_name)), p_value);
}

void Shader::SetUniform(const std::string&& p_name, const float p_value)
{
	glUniform1f(GetUniformLocation(std::move(p_name)), p_value);
}

void Shader::SetUniform(const std::string && p_name, const Core::Maths::vec2f& p_vec2f)
{
	glUniform2f(GetUniformLocation(std::move(p_name)), p_vec2f.x, p_vec2f.y);
}

void Shader::SetUniform(const std::string&& p_name, const Core::Maths::vec3f& p_vec3f)
{
	glUniform3f(GetUniformLocation(std::move(p_name)), p_vec3f.x, p_vec3f.y, p_vec3f.z);
}

void Shader::SetUniform(const std::string&& p_name, const Core::Maths::vec4f& p_vec4f)
{
	glUniform4f(GetUniformLocation(std::move(p_name)), p_vec4f.x, p_vec4f.y, p_vec4f.z, p_vec4f.w);
}

void Shader::SetUniform(const std::string&& p_name, const glm::mat4& p_mat4f)
{
	glUniformMatrix4fv(GetUniformLocation(std::move(p_name)), 1, GL_FALSE, glm::value_ptr(p_mat4f));
}