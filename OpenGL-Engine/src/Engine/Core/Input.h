#pragma once

#include "Engine/Core/Core.h"

namespace Engine {

	class ENGINE_API Input
	{
	public:
		// Static input polling functions
		inline static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		// Implementations of input polling to be done in the platform-specific files
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl () = 0;
		virtual float GetMouseXImpl () = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> s_Instance;

	};

}
