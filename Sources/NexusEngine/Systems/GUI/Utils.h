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
			NX_ENGINE_API NxFr::String GenerateStringId(NxFr::StringView Id, NxFr::StringView Label = "");
			NX_ENGINE_API NxFr::GUID GenerateId(NxFr::StringView Id, NxFr::StringView Label = "");
			NX_ENGINE_API NxFr::StringId ImGuiToNexusId(NxFr::StringView Name);
			NX_ENGINE_API NxFr::StringView ImGuiToNexusName(NxFr::StringView Name);
			NX_ENGINE_API NxFr::String NexusToImGuiId(NxFr::StringView Name, NxFr::GUID Id);
			NX_ENGINE_API NxFr::String NexusToImGuiId(NxFr::StringView Name, NxFr::StringView Id);
			NX_ENGINE_API NxFr::StringView ImGuiIdSeparator();
			NX_ENGINE_API uint32 NexusToImGuiColor(NxFr::Colors::Bits Color);

			NX_ENGINE_API void PushStyle(const Style* Instance);
			NX_ENGINE_API void PopStyle(const Style* Instance);

			NX_ENGINE_API void SetPosition(NxFr::Vector2f Position);
			NX_ENGINE_API void SetSize(NxFr::Vector2f Size);
			NX_ENGINE_API void OffsetLabel(float Label);

			NX_ENGINE_API NxFr::Vector2f Fill(NxFr::Vector2f Reserved = NxFr::Vector2f::Zero, uint64 Space = 0);
			NX_ENGINE_API NxFr::Vector2f Center(NxFr::StringView Text);
			NX_ENGINE_API NxFr::Vector2f Center(NxFr::Vector2f Size, uint64 Space = 0);
			NX_ENGINE_API NxFr::Vector2f Split(uint64 Count);
			NX_ENGINE_API NxFr::Vector2f Align();
			NX_ENGINE_API void SameLine();
		}
	}
}
