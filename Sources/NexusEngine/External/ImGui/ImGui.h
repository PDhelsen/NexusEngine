#pragma once

#include "NexusEngine/External/ImGui/API.h"

namespace NxEn
{
	namespace Imgui
	{
		void Initialize();
		void Shutdown();
		void Tick();
		void Render();
	}
}
