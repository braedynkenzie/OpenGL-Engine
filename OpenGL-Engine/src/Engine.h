#pragma once

// Header file intended to be a single include for engine's client applications

#include <stdio.h>
#include "Engine/Core/Application.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/Log.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseButtonCodes.h"
#include "Engine/Renderer/OrthographicCameraController.h"

#include "Engine/Imgui/ImGuiLayer.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/ScriptableEntity.h"
#include "Engine/Scene/Components.h"
#include "Engine/Scene/Scene.h"


// ---- Renderer files ------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/OrthographicCamera.h"
// --------------------------------
