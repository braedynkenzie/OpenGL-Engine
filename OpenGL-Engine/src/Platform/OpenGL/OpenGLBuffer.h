#pragma once

#include "Engine/Renderer/Buffer.h"

namespace Engine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(unsigned __int32 size);
		OpenGLVertexBuffer(float* vertices, unsigned __int32 size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetData(const void* data, uint32_t size) override;

		const BufferLayout& GetLayout() const override { return m_BufferLayout; }
		void SetLayout(const BufferLayout& vertexBufferLayout) override { m_BufferLayout = vertexBufferLayout; }

	private:
		unsigned __int32 m_RendererID;
		BufferLayout m_BufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned __int32* indices, unsigned __int32 size);
		virtual ~OpenGLIndexBuffer();

		//virtual void SetBufferData() = 0;

		void Bind() const override;
		void Unbind() const override;

		inline unsigned __int32 GetCount() const override { return m_Count; }

	private:
		unsigned __int32 m_RendererID;
		unsigned __int32 m_Count;

	};
}

