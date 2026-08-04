#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Style.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Utils
		{
			NX_ENGINE_API bool IsPanelActive();
			NX_ENGINE_API NxFr::String GenerateId(NxFr::StringView Id, NxFr::StringView Label = "");

			NX_ENGINE_API void PushStyle(const Style* Instance);
			NX_ENGINE_API void PopStyle(const Style* Instance);

			NX_ENGINE_API void SetPosition(const Style* Instance);
			NX_ENGINE_API void SetPosition(NxFr::Vector2f Position);
			NX_ENGINE_API void SetSize(const Style* Instance);
			NX_ENGINE_API void SetSize(NxFr::Vector2f Size);
			NX_ENGINE_API void OffsetLabel(const Style* Instance);
			NX_ENGINE_API void OffsetLabel(float Label);

			NX_ENGINE_API NxFr::Vector2f Fill(NxFr::Vector2f Reserved = NxFr::Vector2f::Zero, uint64 Space = 0);
			NX_ENGINE_API NxFr::Vector2f Center(NxFr::StringView Text);
			NX_ENGINE_API NxFr::Vector2f Center(NxFr::Vector2f Size, uint64 Space = 0);
		}
	}
}
