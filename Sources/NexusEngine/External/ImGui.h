#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#define IM_VEC2_CLASS_EXTRA \
    ImVec2(const NxFr::Vector2f& V) { x = V.x; y = V.y; } \
    operator NxFr::Vector2f() const { return NxFr::Vector2f(x, y); }
#define IM_VEC4_CLASS_EXTRA \
    ImVec4(const NxFr::Color& C) { x = C.r; y = C.g; z = C.b; w = C.a; } \
    operator NxFr::Color() const { return NxFr::Color(x, y, z, w); } \

#include "imgui/imgui.h"

namespace NxEn
{
	namespace Imgui
	{
		void Initialize();
		void Shutdown();
		void Frame();
		void Render();
	}
}
