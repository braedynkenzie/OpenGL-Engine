#pragma once
namespace Engine {

	class VertexBuffer
	{
	public:
		static VertexBuffer* Create(float* vertices, unsigned __int32 size);
		virtual ~VertexBuffer() {}
		
		//virtual void SetBufferData() = 0;
		
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};

	class IndexBuffer
	{
	public:
		static IndexBuffer* Create(unsigned __int32* indices, unsigned __int32 size);
		virtual ~IndexBuffer() {}

		//virtual void SetBufferData() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned __int32 GetCount() const = 0;

	};
}



