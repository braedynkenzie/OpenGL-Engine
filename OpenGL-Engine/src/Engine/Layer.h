#pragma once

#include "Engine\Core.h"
#include "Engine\Events\Event.h"
#include "Engine\Core\Timestep.h"

namespace Engine {

	class ENGINE_API Layer
	{
	public:
		// bool m_Enabled;

		Layer(const std::string& layerName = "Layer");
		virtual ~Layer();

		// Called when the Layer is added to an applications LayerStack
		virtual void OnAttach() {}
		// Called when the Layer is removed from a LayerStack
		virtual void OnDetach() {}
		// Called by the application every frame (when Layer is active) 
		virtual void OnUpdate(Timestep deltaTime) {}
		// Handle events, or let them be passed to the next Layer in the LayerStack
		virtual void OnEvent(Event& event) {}
		// All layers can have GUI using ImGui (defaults to none)
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() { return m_DebugName; }

	protected:
		std::string m_DebugName;

	};

}

