#include "Engine.h"

#include "imgui/imgui.h"

#include <glm\glm\gtc\type_ptr.hpp>
#include <glm\glm\ext\matrix_transform.hpp>

#include "EditorLayer.h"

// ---- Engine Entry Point --------
#include "Engine/Core/EntryPoint.h"
// --------------------------------

namespace Engine {

	class Editor : public Application
	{
	public:
		Editor()
			: Application("Editor Application")
		{
			this->PushLayer(new EditorLayer());
		}

		~Editor() {}
	};

	Application* CreateApplication()
	{
		return new Editor();
	}
}
