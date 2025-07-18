#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace ImGui
	{
		void Initialize(void* Window, NxFr::StringView ConfigPath = "");
		void Shutdown();
		void Frame();
		void Render();
	}
}
