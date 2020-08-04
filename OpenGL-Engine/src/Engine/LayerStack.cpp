#include "EnginePCH.h"
#include "LayerStack.h"

namespace Engine {

	LayerStack::LayerStack()
		: m_LayerInsertLoc(m_Layers.begin())
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsertLoc = m_Layers.emplace(m_LayerInsertLoc, layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);
		// if layer is found in m_Layers, remove it and decrement m_LayerInsert
		if (iterator != m_Layers.end())
		{
			m_Layers.erase(iterator);
			m_LayerInsertLoc--;
			// delete layer; // if we wanted to never use layers after popping them
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		// if overlay is found in m_Layers, remove it
		if (iterator != m_Layers.end())
		{
			// Remove the overlay from m_Layers
			m_Layers.erase(iterator);
			// delete overlay; // if we wanted to never use overlays after popping them
		}
	}

}

