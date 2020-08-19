#include "EnginePCH.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Engine {

	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType dataType)
	{
		switch (dataType)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Matrix3:	return GL_FLOAT;
		case ShaderDataType::Matrix4:	return GL_FLOAT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}
		ENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return -1;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
		//glBindVertexArray(m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		// Assert that the VertexBuffer has already had its layout defined
		ENGINE_CORE_ASSERT(vertexBuffer->GetLayout().GetBufferElements().size() != 0, "The VertexBuffer must have a valid BufferLayout defined before attaching it to a VertexArray!");

		// Bind this VertexArray, then bind the VertexBuffer to it
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		// Set Vertex Attribute Pointers
		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				ShaderDataTypeToOpenGLType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}
		// Also keep track of all VertexBuffers bound to this VertexArray
		m_VertexBuffers.push_back(vertexBuffer);
		// Unbind the VertexArray
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		// Bind this VertexArray, then bind the IndexBuffer to it
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		// Also keep track of the active IndexBuffer bound to this VertexArray
		m_IndexBuffer = indexBuffer;
		// Unbind the VertexArray
		glBindVertexArray(0);
	}

}