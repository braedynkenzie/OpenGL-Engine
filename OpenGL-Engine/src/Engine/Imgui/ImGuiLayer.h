#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"

namespace Engine {

	class ENGINE_API ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		//void OnImGuiRender() override;

		void Begin();
		void End();

		void SetDarkTheme();
	private:
		float m_Time = 0.0f;

	};

}

