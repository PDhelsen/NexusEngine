#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Utils
		{
			NEXUS_ENGINE_API NxFr::Vector2f Fill(NxFr::Vector2f Offset = NxFr::Vector2f::Zero, float Count = 0.0f, bool IncludePadding = true, bool Window = false);
		}
	}
}
