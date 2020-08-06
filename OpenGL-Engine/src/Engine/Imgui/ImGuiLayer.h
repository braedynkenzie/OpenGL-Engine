#pragma once

#include "Engine\Layer.h"
#include "Engine\Events\MouseEvent.h"
#include "Engine\Events\ApplicationEvent.h"
#include "Engine\Events\KeyEvent.h"

namespace Engine {

	class ENGINE_API ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:
		float m_Time = 0.0f;

	private:
		// The following OnEvent functions return whether or not they handled the event
		bool OnMouseMoveEvent(MouseMoveEvent& mmEvent);
		bool OnMouseScrollEvent(MouseScrollEvent& msEvent);
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& mbpEvent);
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& mbrEvent);
		bool OnKeyPressEvent(KeyPressEvent& kpEvent);
		bool OnKeyReleaseEvent(KeyReleaseEvent& krEvent);
		bool OnKeyTypedEvent(KeyTypedEvent& ktEvent);
		bool OnWindowResizeEvent(WindowResizeEvent& wrEvent);

	};

}

