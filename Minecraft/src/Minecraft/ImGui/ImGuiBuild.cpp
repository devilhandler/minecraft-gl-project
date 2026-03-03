#include "mcpch.h"

// in the newest build of ImGui does not need to have the loader chosen, it has it's own loader
// already baked in :]
// #define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>