#pragma once
namespace Engine {

	enum class ShaderDataType 
	{
		None = 0, 
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4, 
		Matrix3, Matrix4,
		Bool
	};
	
	// Function to convert from ShaderDataType to corresponding size in bytes
	static unsigned __int32 ShaderDataTypeSize(ShaderDataType dataType)
	{
		switch (dataType)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Matrix3:	return 4 * 3 * 3;
			case ShaderDataType::Matrix4:	return 4 * 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}
		ENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return -1;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		unsigned __int32 Size;
		unsigned __int32 Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType dataType, const std::string& name, bool normalized = false)
			:Name(name), Type(dataType), Size(ShaderDataTypeSize(dataType)), Offset(0), Normalized(normalized) {}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Matrix3:	return 3 * 3;
				case ShaderDataType::Matrix4:	return 4 * 4;
				case ShaderDataType::Bool:		return 1;
			}
			ENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return -1;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
		: m_BufferElements(elements) 
		{
			// Calculate offsets and stride from given buffer elements
			CalcOffsetsAndStride();
		}

		inline unsigned __int32 GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetBufferElements() const { return m_BufferElements; }

		// Allow iteration through BufferLayout as if over m_BufferElements
		std::vector<BufferElement>::iterator begin() { return m_BufferElements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_BufferElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_BufferElements.end(); }

	private:
		void CalcOffsetsAndStride()
		{
			m_Stride = 0;
			uint32_t offset = 0;
			for (BufferElement& element : m_BufferElements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_BufferElements;
		unsigned __int32 m_Stride;

	};

	class VertexBuffer
	{
	public:
		static VertexBuffer* Create(float* vertices, unsigned __int32 size);
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& vertexBufferLayout) = 0;
	};

	class IndexBuffer
	{
	public:
		static IndexBuffer* Create(unsigned __int32* indices, unsigned __int32 size);
		virtual ~IndexBuffer() = default;

		//virtual void SetBufferData() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned __int32 GetCount() const = 0;

	};
}



