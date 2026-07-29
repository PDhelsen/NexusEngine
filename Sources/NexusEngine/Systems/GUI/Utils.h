#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		namespace Utils
		{
			NX_ENGINE_API NxFr::Vector2f Fill(NxFr::Vector2f Offset = NxFr::Vector2f::Zero, float Count = 0.0f, bool IncludePadding = true, bool Window = false);
			NX_ENGINE_API NxFr::Vector2f Center(float WidgetSize, float Count = 0.0f, bool IncludePadding = true);
			NX_ENGINE_API float Center(NxFr::StringView Text);
			NX_ENGINE_API NxFr::String GenerateId(NxFr::StringView Label = "", NxFr::StringView Id = "");
			NX_ENGINE_API bool IsPanelActive();
		}

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
		}

		template<typename T>
		struct Drawer
		{
			static void Property(const T& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				NxFr::String Buffer = NxFr::StringUtility::ToString<T>(Data);

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				Draw::Text(Buffer, &DrawerStyle);
			}

			static bool Field(T& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String Buffer = NxFr::StringUtility::ToString<T>(Data);
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				DrawerStyle.Flag = DrawerStyle.Flag != 0 ? DrawerStyle.Flag : ImGuiInputTextFlags_EnterReturnsTrue;
				bool Result = Draw::Input(Buffer, ImGuiId, &DrawerStyle);

				NxFr::StringUtility::FromString<T>(Buffer, Data, "");
				return Result;
			}
		};
	}
}
