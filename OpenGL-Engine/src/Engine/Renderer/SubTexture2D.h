#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

namespace Engine {

	class SubTexture2D 
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture2D, const glm::vec2& bottomLeftCoords, const glm::vec2& topRightCoords);

		const glm::vec2* GetTextureCoords() { return m_TextureCoords; }
		const Ref<Texture2D> GetTexture() { return m_FullTexture; }

		static Ref<SubTexture2D> Create(glm::vec2 spriteCoords, glm::vec2 spriteSize, Ref<Texture2D> fullTexture, uint32_t gapPixels);

	private:
		Ref<Texture2D> m_FullTexture;
		glm::vec2 m_TextureCoords[4];
	};
}