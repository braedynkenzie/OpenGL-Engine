#include "TestGameLayer.h"

#include "imgui/imgui.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm\ext\matrix_transform.hpp>

// TEMPORARY
#include "Platform/OpenGL/OpenGLShader.h"
#include <GLFW\include\GLFW\glfw3.h>

TestGameLayer::TestGameLayer()
	: Layer("2D Game Test"), 
	m_ClearColour({ 56.0f / 255, 161.0f / 255, 255.0f / 255, 1.0f }),
	m_CameraController(1280.0f / 720.0f, 1.8f, true),
	m_StartingRabbitPosition(-2.4f, -1.1f, 0.0f),
	m_RabbitPosition(m_StartingRabbitPosition),
	m_RabbitVelocity(0.0f, 0.0f, 0.0f),
	m_RabbitJumpPower(4.2f), 
	m_RabbitGravity(6.0f),
	m_RabbitAngle(0.0f),
	m_RabbitAirSpeed(1.2f),
	m_ObstacleSpeed(1.0f),
	m_MaxFallSpeed(-10.0f),
	m_ModTime(0.0f)
{
}

void TestGameLayer::OnAttach()
{
	// Load textures
	m_RabbitNormalTexture = Engine::Texture2D::Create("assets/textures/rabbit_sprite1.png");
	m_RabbitJumpingTexture = Engine::Texture2D::Create("assets/textures/rabbit_sprite2.png");
	m_RabbitTexture = m_RabbitNormalTexture;
	m_GrassTexture = Engine::Texture2D::Create("assets/textures/grass_sprite.png");
	m_GroundTexture = Engine::Texture2D::Create("assets/textures/ground_sprite.png");
	m_BackgroundTexture = Engine::Texture2D::Create("assets/textures/background_image.png");

	m_TreeTextures = {
		Engine::Texture2D::Create("assets/textures/tree_sprite1.png"),
		Engine::Texture2D::Create("assets/textures/tree_sprite2.png"),
		Engine::Texture2D::Create("assets/textures/tree_sprite3.png"),
		Engine::Texture2D::Create("assets/textures/tree_sprite4.png"),
		Engine::Texture2D::Create("assets/textures/tree_sprite5.png"),
		Engine::Texture2D::Create("assets/textures/tree_sprite6.png"),
		Engine::Texture2D::Create("assets/textures/tree_sprite7.png"),
		Engine::Texture2D::Create("assets/textures/tree_sprite8.png"),
	};

	AddNewTree();
	AddNewBackgroundTree();
}

void TestGameLayer::AddNewTree()
{
	int newTreeIndex = rand() % 8;
	float xOffset = ((float)(rand() % 100) / 50.0f);
	float yOffset = ((float)(rand() % 100) / 150.0f);
	switch (newTreeIndex)
	{
	case 0:
		m_Trees.push_back({ { 8.0f - xOffset, -0.55f - yOffset, xOffset / 2.1f }, { 0.8f, 1.4f }, m_TreeTextures[newTreeIndex] });
		break;
	case 1:
		m_Trees.push_back({ { 8.0f - xOffset, -0.5f - yOffset, xOffset / 2.1f }, { 2.6f, 2.2f }, m_TreeTextures[newTreeIndex] });
		break;
	case 2:
		m_Trees.push_back({ { 8.0f - xOffset, -0.6f - yOffset, xOffset / 2.1f }, { 0.8f, 1.4f }, m_TreeTextures[newTreeIndex] });
		break;
	case 3:
		m_Trees.push_back({ { 8.0f - xOffset, -0.55f - yOffset, xOffset / 2.1f }, { 0.8f, 1.4f }, m_TreeTextures[newTreeIndex] });
		break;
	case 4:
		m_Trees.push_back({ { 8.0f - xOffset, -1.1f - yOffset, xOffset / 2.1f }, { 0.5f, 0.4f }, m_TreeTextures[newTreeIndex] });
		break;
	case 5:
		m_Trees.push_back({ { 8.0f - xOffset, -1.1f - yOffset, xOffset / 2.1f }, { 0.5f, 0.4f }, m_TreeTextures[newTreeIndex] });
		break;
	case 6:
		m_Trees.push_back({ { 8.0f - xOffset, -0.4f - yOffset, xOffset / 2.1f }, { 0.8f, 2.0f }, m_TreeTextures[newTreeIndex] });
		break;
	case 7:
		m_Trees.push_back({ { 8.0f - xOffset, -0.4f - yOffset, xOffset / 2.1f }, { 2.6f, 2.2f }, m_TreeTextures[newTreeIndex] });
		break;
	}
}

void TestGameLayer::AddNewBackgroundTree()
{
	int newTreeIndex = rand() % 8;
	float xOffset = ((float)(rand() % 100) / 50.0f);
	float yOffset = ((float)(rand() % 100) / 400.0f);
	switch (newTreeIndex)
	{
	case 0:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -0.55f - yOffset, -xOffset / 2.1f }, glm::vec2(0.8f, 1.4f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	case 1:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -0.6f - yOffset, -xOffset / 2.1f }, glm::vec2(2.6f, 2.2f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	case 2:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -0.7f - yOffset, -xOffset / 2.1f }, glm::vec2(0.8f, 1.4f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	case 3:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -0.65f - yOffset, -xOffset / 2.1f }, glm::vec2(0.8f, 1.4f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	case 4:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -1.45f - yOffset, -xOffset / 2.1f }, glm::vec2(0.5f, 0.4f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	case 5:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -1.45f - yOffset, -xOffset / 2.1f }, glm::vec2(0.5f, 0.4f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	case 6:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -0.5f - yOffset, -xOffset / 2.1f }, glm::vec2(0.8f, 2.0f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	case 7:
		m_BackgroundTrees.push_back({ { 8.0f - xOffset, -0.4f - yOffset, -xOffset / 2.1f }, glm::vec2(2.6f, 2.2f) * 0.6f, m_TreeTextures[newTreeIndex] });
		break;
	}
}

void TestGameLayer::OnDetach()
{
}

void TestGameLayer::OnUpdate(Engine::Timestep deltaTime)
{
	// ------------------------------------------------------------------------------------------------------
	// Update section ---------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------------------

	if ((float)deltaTime > 0.5f)
		deltaTime = 0.5f;
	m_ModTime += deltaTime;
	m_ModTime = fmod(m_ModTime, 1.0f);

	// Process any camera movement or zoom changes
	m_CameraController.OnUpdate(deltaTime);

	// Update the rabbits position
	if (m_RabbitVelocity.y > m_MaxFallSpeed && m_RabbitPosition.y > m_StartingRabbitPosition.y)
		m_RabbitVelocity.y -= m_RabbitGravity *(float)deltaTime;
	if (m_RabbitPosition.y < m_StartingRabbitPosition.y)
	{
		// Rabbit landed
		m_RabbitVelocity.y = 0.0f;
		m_RabbitPosition.y = m_StartingRabbitPosition.y;
		m_RabbitAngle = 0.0f;
		m_RabbitTexture = m_RabbitNormalTexture;
	}
	m_RabbitPosition += m_RabbitVelocity * (float)deltaTime;

	// Update the rabbit's angle while jumping
	if((m_RabbitAngle < 20.0f && m_RabbitVelocity.y > 0.0f)   // rabbit rising and angle < 20 degrees
	|| (m_RabbitAngle > -10.0f && m_RabbitVelocity.y < 0.0f)) // or rabbit falling and angle > -10 degrees
		m_RabbitAngle += m_RabbitVelocity.y;


	// Update the tree positions
	for (TreeData& tree : m_Trees)
	{
		tree.Position.x -= m_ObstacleSpeed * (float)deltaTime;
		// Clear off-screen trees
		if (tree.Position.x < -6.0f)
			m_Trees.erase(m_Trees.begin());
	}
	for (TreeData& backgroundTree : m_BackgroundTrees)
	{
		backgroundTree.Position.x -= m_ObstacleSpeed * 0.7f * (float)deltaTime;
		// Clear off-screen trees
		if (backgroundTree.Position.x < -6.0f)
			m_BackgroundTrees.erase(m_BackgroundTrees.begin());
	}

	// Add a new tree if there's room
	if (m_Trees.back().Position.x < 6.0f)
		AddNewTree();
	if (m_BackgroundTrees.back().Position.x < 6.5f)
		AddNewBackgroundTree();

	// ------------------------------------------------------------------------------------------------------
	// Render section ---------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------------------
	Engine::RenderCommand::SetClearColour(m_ClearColour);
	Engine::RenderCommand::Clear();

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	// Draw background
	Engine::Renderer2D::DrawTexturedQuad({ 0.0f, 0.0f, -0.99f }, { 16.0f * 0.7f, 9.0f * 0.7f }, m_BackgroundTexture);

	// Draw back layer of trees
	for (auto backgroundTree : m_BackgroundTrees)
		Engine::Renderer2D::DrawTexturedQuad(backgroundTree.Position, backgroundTree.Scale, backgroundTree.Texture);

	// Draw front layers of trees
	for (auto tree : m_Trees)
		Engine::Renderer2D::DrawTexturedQuad(tree.Position, tree.Scale, tree.Texture);

	// Draw the rabbit
	Engine::Renderer2D::DrawTexturedQuad(m_RabbitPosition, { 0.6f, 0.6f }, m_RabbitTexture, m_RabbitAngle);

	// Draw the grass and ground
	for (int x = -4; x < 12; x++)
	{
		// Background
		Engine::Renderer2D::DrawTexturedQuad({ (x * 0.7f) - (m_ModTime * 0.7f), -1.9f, -0.9f}, { 2.4f, 2.4f }, m_GrassTexture);
		// Foreground
		Engine::Renderer2D::DrawTexturedQuad({ x - m_ModTime, -2.7f, 0.0f}, { 3.2f, 3.2f }, m_GrassTexture);
	}

	Engine::Renderer2D::EndScene();
}

void TestGameLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Clear Colour", glm::value_ptr(m_ClearColour));
	ImGui::End();
}

void TestGameLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);

	Engine::EventDispatcher dispatcher = Engine::EventDispatcher(event);
	dispatcher.Dispatch<Engine::KeyPressEvent>(ENGINE_BIND_EVENT_FUNC(TestGameLayer::OnKeyPressedEvent));
}

bool TestGameLayer::OnKeyPressedEvent(Engine::KeyPressEvent kpEvent)
{
	if (kpEvent.GetKeyCode() == ENGINE_KEY_SPACE)
	{
		if (m_RabbitVelocity.y == 0.0f)
		{
			m_RabbitVelocity.y += m_RabbitJumpPower;
			m_RabbitAngle += 0.2f;
			m_RabbitTexture = m_RabbitJumpingTexture;
			return true;
		}
	}
	return false;
}