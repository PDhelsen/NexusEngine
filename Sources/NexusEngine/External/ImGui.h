#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace Imgui
	{
		void Initialize(void* Window);
		void Shutdown();
		void Frame();
		void Render();
	}
}
