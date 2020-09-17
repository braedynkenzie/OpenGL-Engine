#include "EnginePCH.h"
#include "SubTexture2D.h"
#include "EnginePCH.h"

namespace Engine {

	Engine::SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture2D, const glm::vec2& bottomLeftCoords, const glm::vec2& topRightCoords)
		: m_FullTexture(texture2D)
	{
		m_TextureCoords[0] = { bottomLeftCoords.x, bottomLeftCoords.y };
		m_TextureCoords[1] = { topRightCoords.x, bottomLeftCoords.y };
		m_TextureCoords[2] = { topRightCoords.x, topRightCoords.y };
		m_TextureCoords[3] = { bottomLeftCoords.x, topRightCoords.y };
	}

	Ref<SubTexture2D> SubTexture2D::Create(glm::vec2 spriteCoords, glm::vec2 spriteSize, Ref<Texture2D> fullTexture, uint32_t gapPixels)
	{
		glm::vec2 bottomLeftCoords = glm::vec2((spriteCoords.x * spriteSize.x) / fullTexture->GetWidth(), (spriteCoords.y * spriteSize.y) / fullTexture->GetHeight());
		glm::vec2 topRightCoords = glm::vec2((spriteCoords.x * spriteSize.x + spriteSize.x - gapPixels) / fullTexture->GetWidth(), (spriteCoords.y * spriteSize.y + spriteSize.y - gapPixels) / fullTexture->GetHeight());
		return Engine::CreateRef<SubTexture2D>(fullTexture, bottomLeftCoords, topRightCoords);
	}

}
