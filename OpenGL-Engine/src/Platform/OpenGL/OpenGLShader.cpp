#include "EnginePCH.h"
#include "OpenGLShader.h"

#include <glm\gtc\type_ptr.hpp>
#include <fstream>

// TODO: move this to OpenGLShader.h once we aren't including that file in sandbox
#include <glad/glad.h>

namespace Engine {

	static GLenum ShaderTypeFromString(const std::string& typeStr)
	{
		if (typeStr == "vertex")
			return GL_VERTEX_SHADER;
		else if (typeStr == "fragment")
			return GL_FRAGMENT_SHADER;

		ENGINE_CORE_ERROR("Unknown shader type: {0}", typeStr);
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		ENGINE_PROFILE_FUNCTION();

		{
			ENGINE_PROFILE_SCOPE("Read shader file and compile");
			std::string& fileContents = ReadFile(filepath);
			std::unordered_map<GLenum, std::string> shadersMap = ProcessShaderSource(fileContents);
			Compile(shadersMap);
		}

		// Need to extract file name from filepath
		// ex. .../assets/shaders/TexturedQuad.glsl  
		//			->  m_Name = "TexturedQuad";
		auto lastSlashPos = filepath.find_last_of("\\/");
		auto firstLetterPos = (lastSlashPos == std::string::npos) ? 
			0 : lastSlashPos + 1;
		auto lastDotPos = filepath.rfind('.');
		auto shaderNameLength = (lastDotPos == std::string::npos) ? 
			filepath.length() - firstLetterPos : lastDotPos - firstLetterPos;
		const std::string shaderName = filepath.substr(firstLetterPos, shaderNameLength);
		m_Name = shaderName;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vsSource, const std::string& fsSource)
		: m_Name(name)
	{
		ENGINE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shadersMap;
		shadersMap[GL_VERTEX_SHADER] = vsSource;
		shadersMap[GL_FRAGMENT_SHADER] = fsSource;
		Compile(shadersMap);
	}

	OpenGLShader::~OpenGLShader()
	{
		ENGINE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, const glm::int32 value)
	{
		ENGINE_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, const glm::float32 value)
	{
		ENGINE_PROFILE_FUNCTION();

		UploadUniformFloat1(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3 matrix)
	{
		ENGINE_PROFILE_FUNCTION();

		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4 matrix)
	{
		ENGINE_PROFILE_FUNCTION();

		UploadUniformMat4(name, matrix);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3 float3)
	{
		ENGINE_PROFILE_FUNCTION();

		UploadUniformFloat3(name, float3);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4 float4)
	{
		ENGINE_PROFILE_FUNCTION();

		UploadUniformFloat4(name, float4);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec4)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, vec4.r, vec4.g, vec4.b, vec4.a);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec3)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, vec3.r, vec3.g, vec3.b);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec2)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, vec2.x, vec2.y);
	}

	void OpenGLShader::UploadUniformFloat1(const std::string& name, float val)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, val);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int val)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, val);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		ENGINE_PROFILE_FUNCTION();

		std::string fileContents;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			// Move file pointer to end of file then resize fileContents to length of file
			in.seekg(0, std::ios::end);
			fileContents.resize(in.tellg());
			// Move file pointer back to the start of the file
			in.seekg(0, std::ios::beg);
			// Read file into string fileContents
			in.read(&fileContents[0], fileContents.size());
			in.close();
		}
		else
		{
			ENGINE_CORE_ERROR("Shader source file could not be read! Filepath: {0}", filepath);
		}
		return fileContents;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ProcessShaderSource(const std::string& shaderSource)
	{
		ENGINE_PROFILE_FUNCTION();

		// Split the shaderSource string into a map of GLenums (the shader types) and corresponding string sources
		std::unordered_map<GLenum, std::string> shadersMap;

		const char* shaderTypeToken = "#type";
		size_t tokenLength = strlen(shaderTypeToken);
		size_t position = shaderSource.find(shaderTypeToken, 0);
		while (position != std::string::npos)
		{
			size_t lineEnd = shaderSource.find_first_of("\r\n", position);
			ENGINE_CORE_ASSERT(lineEnd != std::string::npos, "Syntax error in shader source file!");
			size_t begin = position + tokenLength + 1;
			std::string typeStr = shaderSource.substr(begin, lineEnd - begin);

			size_t nextLinePos = shaderSource.find_first_not_of("\r\n", lineEnd);
			position = shaderSource.find(shaderTypeToken, nextLinePos);
			shadersMap[ShaderTypeFromString(typeStr)] = shaderSource.substr(nextLinePos, 
				position - (nextLinePos == std::string::npos ? shaderSource.size() - 1 : nextLinePos));
		}
		return shadersMap;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shadersMap)
	{
		ENGINE_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		const int maxNumShaders = 2; // for now
		ENGINE_CORE_ASSERT(shadersMap.size() <= maxNumShaders, "Trying to compile too many shaders at once!");
		ENGINE_CORE_ASSERT(shadersMap.size() != 0,			   "No shaders to compile!")
		std::array<GLenum, maxNumShaders> shaderIDs;
		int shaderIDIndex = 0;
		for (auto& keyValue : shadersMap)
		{
			GLenum shaderType = keyValue.first;
			const std::string& shaderSource = keyValue.second;

			GLuint shader = glCreateShader(shaderType);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = (const GLchar*)shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore
				glDeleteShader(shader);

				// Error message
				ENGINE_CORE_ERROR("{0}", infoLog.data());
				ENGINE_CORE_ASSERT(false, "hader compilation failed!");
				break;
			}
			// Attach our shaders to our program
			glAttachShader(program, shader);
			shaderIDs[shaderIDIndex++] = shader;
		}
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			
			for (auto& shaderID : shaderIDs)
			{
				glDetachShader(program, shaderID);
				glDeleteShader(shaderID);
			}

			// Error message
			ENGINE_CORE_ERROR("{0}", infoLog.data());
			ENGINE_CORE_ASSERT(false, "Shader linking failed!");
			return;
		}

		for (auto& shaderID : shaderIDs)
			glDetachShader(program, shaderID);

		m_RendererID = program;
	}

}