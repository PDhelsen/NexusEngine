#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Styles.h"
#include "NexusEngine/Systems/GUI/Utils.h"
#include "NexusEngine/Systems/GUI/Misc/Constants.h"
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
				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;
				Draw::Text(Data, &DrawerStyle);
			}

			static bool Field(NxFr::String& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);

				Style DrawerStyle = Style::Copy(Visual);
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
				Style DrawerStyle = Style::Copy(Visual);
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
				Style DrawerStyle = Style::Copy(Visual);
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
				NxFr::String ImGuiId = Utils::GenerateId(Label);

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = -1.0f;

				ImGui::BeginDisabled();
				bool Value = Data;
				ImGui::Checkbox(ImGuiId.C(), &Value);
				ImGui::EndDisabled();
			}

			static bool Field(bool& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);

				Style DrawerStyle = Style::Copy(Visual);
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
				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = 0.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Drawer<T>::Property(Data[Index], IndexStr + ":", &DrawerStyle);
				}
			}

			static bool Field(NxFr::Array<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);
				float ButtonSize = Styles::WidthButton();
				uint64 Count = Data.GetCount();
				bool Result = false;

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = 0.0f;

				DrawerStyle.Width = Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Count:").x + ButtonSize * 2.0f, 0.0f), 3, false).x;
				if (Drawer<uint64>::Field(Count, "Count:", ImGuiId + "Count", &DrawerStyle))
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

				DrawerStyle.Width = -1.0f;
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
				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = 0.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
				{
					NxFr::String IndexStr = NxFr::StringUtility::ToString(Index);
					Drawer<T>::Property(Data[Index], IndexStr + ":", &DrawerStyle);
				}
			}

			static bool Field(NxFr::List<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);
				float ButtonSize = Styles::WidthButton();
				uint64 Count = Data.GetCount();
				bool Result = false;

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = 0.0f;

				DrawerStyle.Width = Utils::Fill(NxFr::Vector2f(ImGui::CalcTextSize("Count:").x + ButtonSize * 2.0f, 0.0f), 3, false).x;
				if (Drawer<uint64>::Field(Count, "Count:", ImGuiId + "Count", &DrawerStyle))
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

				DrawerStyle.Width = -1.0f;
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
				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = 0.0f;
				DrawerStyle.Width = -1.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (const auto& It : Data)
				{
					GUI::Drawer<T>::Property(It, "", &DrawerStyle);
				}
			}

			static bool Field(NxFr::Set<T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);
				float ButtonSize = Styles::WidthButton();

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = 0.0f;
				DrawerStyle.Width = -1.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + Utils::Fill(NxFr::Vector2f(ButtonSize * 2.0f, 0.0f), 1, false).x);
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
				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = -1.0f;
				DrawerStyle.Width = -1.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);

				for (const auto& It : Data)
				{
					NxFr::String KeyStr = NxFr::StringUtility::ToString(It.Key);
					GUI::Drawer<T>::Property(It.Value, KeyStr + ":", &DrawerStyle);
				}
			}

			static bool Field(NxFr::Dictionary<K, T>& Data, NxFr::StringView Label = "", NxFr::StringView Id = "", const Style* Visual = nullptr)
			{
				NxFr::String ImGuiId = Utils::GenerateId(Label, Id);
				float ButtonSize = Styles::WidthButton();

				Style DrawerStyle = Style::Copy(Visual);
				Draw::Label(Label, &DrawerStyle);
				DrawerStyle.Position.x = ImGui::GetCursorPosX();
				DrawerStyle.WidthLabel = -1.0f;
				DrawerStyle.Width = -1.0f;

				Drawer<uint64>::Property(Data.GetCount(), "Count:", &DrawerStyle);
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + Utils::Fill(NxFr::Vector2f(ButtonSize * 2.0f, 0.0f), 1, false).x);
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

namespace YAML
{
	template<>
	struct convert<NxEn::GUI::Style>
	{
		static Node encode(const NxEn::GUI::Style& rhs)
		{
			Node node;
			node["StyleType"] = (int32)rhs.StyleType;
			node["Width"] = rhs.Width;
			node["WidthLabel"] = rhs.WidthLabel;
			node["Height"] = rhs.Font;
			node["Color"] = rhs.Color;
			node["ColorText"] = rhs.ColorText;
			node["ColorBackground"] = rhs.ColorBackground;
			node["ColorBorder"] = rhs.ColorBorder;
			node["Alpha"] = rhs.Alpha;
			node["Align"] = rhs.Align;
			node["Spacing"] = rhs.Spacing;
			node["Padding"] = rhs.Padding;
			node["Rounding"] = rhs.Rounding;
			node["Border"] = rhs.Border;
			node["Font"] = rhs.Font;
			node["Flag"] = rhs.Flag;
			return node;
		}

		static bool decode(const Node& node, NxEn::GUI::Style& rhs)
		{
			rhs.StyleType = (NxEn::GUI::Style::Type)node["StyleType"].as<int32>();
			rhs.Width = node["Width"].as<float>();
			rhs.WidthLabel = node["WidthLabel"].as<float>();
			rhs.Height = node["Height"].as<float>();
			rhs.Color = node["Color"].as<NxFr::Color>();
			rhs.ColorText = node["ColorText"].as<NxFr::Color>();
			rhs.ColorBackground = node["ColorBackground"].as<NxFr::Color>();
			rhs.ColorBorder = node["ColorBorder"].as<NxFr::Color>();
			rhs.Alpha = node["Alpha"].as<float>();
			rhs.Align = node["Align"].as<NxFr::Vector2f>();
			rhs.Spacing = node["Spacing"].as<NxFr::Vector2f>();
			rhs.Padding = node["Padding"].as<NxFr::Vector2f>();
			rhs.Rounding = node["Rounding"].as<float>();
			rhs.Border = node["Border"].as<float>();
			rhs.Font = node["Font"].as<float>();
			rhs.Flag = node["Flag"].as<uint64>();
			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxEn::GUI::Style& rhs)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "StyleType" << YAML::Value << (int32)rhs.StyleType;
		out << YAML::Key << "Width" << YAML::Value << rhs.Width;
		out << YAML::Key << "WidthLabel" << YAML::Value << rhs.WidthLabel;
		out << YAML::Key << "Height" << YAML::Value << rhs.Height;
		out << YAML::Key << "Color" << YAML::Value << rhs.Color;
		out << YAML::Key << "ColorText" << YAML::Value << rhs.ColorText;
		out << YAML::Key << "ColorBackground" << YAML::Value << rhs.ColorBackground;
		out << YAML::Key << "ColorBorder" << YAML::Value << rhs.ColorBorder;
		out << YAML::Key << "Alpha" << YAML::Value << rhs.Alpha;
		out << YAML::Key << "Align" << YAML::Value << rhs.Align;
		out << YAML::Key << "Spacing" << YAML::Value << rhs.Spacing;
		out << YAML::Key << "Padding" << YAML::Value << rhs.Padding;
		out << YAML::Key << "Rounding" << YAML::Value << rhs.Rounding;
		out << YAML::Key << "Border" << YAML::Value << rhs.Border;
		out << YAML::Key << "Font" << YAML::Value << rhs.Font;
		out << YAML::Key << "Flag" << YAML::Value << rhs.Flag;
		out << YAML::EndMap;
		return out;
	}
}
