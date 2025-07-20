#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace ImGui
	{
		void Initialize(void* Window);
		void Shutdown();
		void Frame();
		void Render();

		void LoadConfig(NxFr::StringView ConfigPath);
		void SaveConfig(NxFr::StringView ConfigPath);
	}
}
