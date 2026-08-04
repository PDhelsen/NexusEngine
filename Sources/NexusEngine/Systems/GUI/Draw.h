#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Style.h"
#include "NexusEngine/Systems/GUI/Utils.h"
#include "NexusEngine/Systems/GUI/Misc/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Draw
		{
			NX_ENGINE_API void Label(NxFr::StringView Data, const Style* Visual);
			NX_ENGINE_API void Label(NxFr::StringView Data);
			NX_ENGINE_API void Text(NxFr::StringView Data, const Style* Visual);
			NX_ENGINE_API void Text(NxFr::StringView Data);
			NX_ENGINE_API bool Input(NxFr::String& Data, NxFr::StringView Id, const Style* Visual);
			NX_ENGINE_API bool Input(NxFr::String& Data, NxFr::StringView Id, uint64 Flag = 0);
			NX_ENGINE_API bool Button(NxFr::StringView Data, const Style* Visual);
			NX_ENGINE_API bool Button(NxFr::StringView Data, NxFr::Vector2f Size = NxFr::Vector2f::Zero);
			NX_ENGINE_API bool CheckBox(NxFr::StringView Text, bool& Data, const Style* Visual);
			NX_ENGINE_API bool CheckBox(NxFr::StringView Text, bool& Data);

			NX_ENGINE_API void Space(const Style* Visual);
			NX_ENGINE_API void Space(NxFr::Vector2f Size = NxFr::Vector2f::Zero);
			NX_ENGINE_API void Separator(const Style* Visual);
			NX_ENGINE_API void Separator();
		}

		template<typename T>
		struct Drawer
		{
			static void Property(const T& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String Buffer = NxFr::StringUtility::ToString<T>(Data);

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				Draw::Text(Buffer);
			}

			static bool Field(T& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				Style::Scope _ = Visual;
				NxFr::String Buffer = NxFr::StringUtility::ToString<T>(Data);
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);

				Utils::SetPosition(Visual);
				Draw::Label(Label);
				Utils::OffsetLabel(Visual);
				Utils::SetSize(Visual);
				bool Result = Draw::Input(Buffer, ImGuiId, Visual ? Visual->Flag : ImGuiInputTextFlags_EnterReturnsTrue);

				NxFr::StringUtility::FromString<T>(Buffer, Data, "");
				return Result;
			}
		};
	}
}
