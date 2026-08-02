#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Draw.h"
#include "NexusEngine/Systems/GUI/Misc/Styles.h"
#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEn
{
	namespace GUI
	{
		template<>
		struct Drawer<NxFr::String>
		{
			static void Property(const NxFr::String& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				Draw::Text(Data, &DrawerStyle);
			}

			static bool Field(NxFr::String& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				DrawerStyle.Flag = DrawerStyle.Flag != 0 ? DrawerStyle.Flag : ImGuiInputTextFlags_EnterReturnsTrue;
				bool Result = Draw::Input(Data, ImGuiId, &DrawerStyle);

				return Result;
			}
		};

		template<>
		struct Drawer<NxFr::StringView>
		{
			static void Property(const NxFr::StringView& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				Draw::Text(Data, &DrawerStyle);
			}

			static bool Field(NxFr::StringView& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NX_ASSERT(false, Default, "Unsupported");
				return false;
			}
		};

		template<>
		struct Drawer<const char*>
		{
			static void Property(const char*& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				Draw::Text(Data, &DrawerStyle);
			}

			static bool Field(const char*& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NX_ASSERT(false, Default, "Unsupported");
				return false;
			}
		};

		template<>
		struct Drawer<bool>
		{
			static void Property(const bool& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId("", Label);

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;

				ImGui::BeginDisabled();
				bool Value = Data;
				ImGui::Checkbox(ImGuiId.C(), &Value);
				ImGui::EndDisabled();
			}

			static bool Field(bool& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				DrawerStyle.Flag = DrawerStyle.Flag != 0 ? DrawerStyle.Flag : ImGuiInputTextFlags_EnterReturnsTrue;
				bool Result = ImGui::Checkbox(ImGuiId.C(), &Data);

				return Result;
			}
		};

		template<typename T>
		struct Drawer<NxFr::Array<T>>
		{
			static void Property(const NxFr::Array<T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = 0.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Drawer<T>::Property(Data[Index], IndexStr + ":", &DrawerStyle);
				}
			}

			static bool Field(NxFr::Array<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();
				uint64 Count = Data.GetCount();
				bool Result = false;

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = 0.0f;

				DrawerStyle.Size.x = Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Count:").x + ButtonSize * 2.0f, 0.0f), 2).x;
				if (Drawer<uint64>::Field(Count, "Count:", "Count" + ImGuiId, &DrawerStyle))
				{
					NxFr::ContainerUtility::Resize(Data, Count);
				}
				ImGui::SameLine();
				if (ImGui::Button(("Add" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					NxFr::ContainerUtility::Resize(Data, Data.GetCount() + 1);
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					NxFr::ContainerUtility::Resize(Data, Data.GetCount() - 1);
				}

				DrawerStyle.Size.x = -1.0f;
				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Result |= GUI::Drawer<T>::Field(Data[Index], IndexStr + ":", ImGuiId + IndexStr, &DrawerStyle);
				}

				return Result;
			}
		};

		template<typename T>
		struct Drawer<NxFr::List<T>>
		{
			static void Property(const NxFr::List<T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = 0.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Drawer<T>::Property(Data[Index], IndexStr + ":", &DrawerStyle);
				}
			}

			static bool Field(NxFr::List<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();
				uint64 Count = Data.GetCount();
				bool Result = false;

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = 0.0f;

				DrawerStyle.Size.x = Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Count:").x + ButtonSize * 2.0f, 0.0f), 2).x;
				if (Drawer<uint64>::Field(Count, "Count:", "Count" + ImGuiId, &DrawerStyle))
				{
					NxFr::ContainerUtility::Resize(Data, Count);
				}
				ImGui::SameLine();
				if (ImGui::Button(("Add" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					Data.AppendConstruct();
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					Data.RemoveLast();
				}

				DrawerStyle.Size.x = -1.0f;
				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Result |= GUI::Drawer<T>::Field(Data[Index], IndexStr + ":", ImGuiId + IndexStr, &DrawerStyle);
				}

				return Result;
			}
		};

		template<typename T>
		struct Drawer<NxFr::Set<T>>
		{
			static void Property(const NxFr::Set<T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = 0.0f;
				DrawerStyle.Size.x = -1.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (const auto& It : Data)
				{
					GUI::Drawer<T>::Property(It, "", &DrawerStyle);
				}
			}

			static bool Field(NxFr::Set<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = 0.0f;

				DrawerStyle.Size.x = Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Count:").x + ButtonSize * 2.0f, 0.0f), 2).x;
				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);
				ImGui::SameLine();
				if (ImGui::Button(("Add" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
						{
							Data.Append(NxFr::StringUtility::FromString<T>(Value));
						});
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
						{
							Data.Remove(NxFr::StringUtility::FromString<T>(Value));
						});
				}

				for (const auto& It : Data)
				{
					GUI::Drawer<T>::Property(It, "", &DrawerStyle);
				}

				return false;
			}
		};

		template<typename K, typename T>
		struct Drawer<NxFr::Dictionary<K, T>>
		{
			static void Property(const NxFr::Dictionary<K, T>& Data, NxFr::StringView Label = "", const Style* Visual = nullptr)
			{
				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = GUI::Styles::WidthLabel();
				DrawerStyle.Size.x = -1.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (const auto& It : Data)
				{
					NxFr::String KeyStr = NxFr::StringUtility::ToString(It.Key);
					GUI::Drawer<T>::Property(It.Value, KeyStr + ":", &DrawerStyle);
				}
			}

			static bool Field(NxFr::Dictionary<K, T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Id, Label);
				float ButtonSize = Styles::WidthButton();

				Style DrawerStyle = Visual ? *Visual : GUI::Styles::Default();
				if (!Visual)
				{
					DrawerStyle.Label = GUI::Styles::WidthLabel();
				}
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.Label = 0.0f;

				DrawerStyle.Size.x = Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Count:").x + ButtonSize * 2.0f, 0.0f), 2).x;
				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);
				ImGui::SameLine();
				if (ImGui::Button(("Add" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
						{
							Data.AppendConstruct(NxFr::StringUtility::FromString<K>(Value));
						});
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove" + ImGuiId).C(), { ButtonSize, 0.0f }))
				{
					InputTextPopup* Popup = InputTextPopup::GetInstance();
					Popup->RegisterCallback([&](NxFr::StringView Value)
						{
							Data.Remove(NxFr::StringUtility::FromString<K>(Value));
						});
				}

				for (auto& It : Data)
				{
					NxFr::String KeyStr = NxFr::StringUtility::ToString(It.Key);
					GUI::Drawer<T>::Field(It.Value, KeyStr + ":", ImGuiId + KeyStr, &DrawerStyle);
				}

				return false;
			}
		};
	}
}
