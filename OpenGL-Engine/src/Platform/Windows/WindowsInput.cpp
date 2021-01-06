#include "EnginePCH.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Engine {

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto keyState = glfwGetKey(glfwWindow, static_cast<int32_t>(keyCode));
		bool keyIsPressed = (keyState == GLFW_PRESS || keyState == GLFW_REPEAT);
		return keyIsPressed;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto mouseButtonState = glfwGetMouseButton(glfwWindow, static_cast<int32_t>(button));
		return mouseButtonState == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(glfwWindow, &xPos, &yPos);
		return { xPos, yPos };
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

}
