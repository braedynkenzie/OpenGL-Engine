#pragma once

#include "Engine/Core.h"
#include "Engine/Layer.h"

namespace Engine {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		// Allows to iterate over LayerStack as if iterating over m_Layers
		std::vector<Layer*>::iterator begin() { return m_Layers.begin();  }
		std::vector<Layer*>::iterator end() { return m_Layers.end();  }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsertLoc;

	};

}
