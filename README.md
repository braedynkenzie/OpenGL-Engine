# OpenGL-Engine

This project is my introduction to interactive 3D graphics/game engine design. The plan is to first implement a working 2D engine on Windows using OpenGL, then moving on to making a full 3D PBR renderer and scripting language support, then eventually adding support for Vulkan, DirectX, perhaps other graphics APIs.

# Current project state
The engine currently has working 2D rendering functionality using an dynamic orthographic camera, and simple GUI support to expose parameters using ImGui.   

# Next steps
- Further optimizing the 2D rendering pipeline 
- Adding a proper 3D renderer implementation
- Adding a scripting language (most likely going to be using Lua)
- Physically Based Rendering (PBR)  
- Animation support
- Audio systems

# Basic demos 
Transformed, textured quads
![](Sandbox/rabbit_forest.gif)
Single texture atlas/spritesheet 
![](Sandbox/top_down_rpg_map.gif)
