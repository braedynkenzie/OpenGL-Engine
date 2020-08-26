#include "EnginePCH.h"
#include "WindowsInput.h"

#include "Engine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Engine {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto keyState = glfwGetKey(glfwWindow, keyCode);
		bool keyIsPressed = (keyState == GLFW_PRESS || keyState == GLFW_REPEAT);
		return keyIsPressed;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto mouseButtonState = glfwGetMouseButton(glfwWindow, button);
		return mouseButtonState == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(glfwWindow, &xPos, &yPos);
		return { xPos, yPos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

}
