#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace GUI
	{
		struct Style;

		struct Scope
		{
		public:
			NEXUS_ENGINE_API Scope(NxFr::StringId Id);
			NEXUS_ENGINE_API Scope(const Style* Instance);
			NEXUS_ENGINE_API ~Scope();

		private:
			NxFr::StringId Id;
			const Style* Instance;
		};

		struct Style
		{
		public:
			enum class Preset
			{
				Text, Button, Panel
			};

			NEXUS_ENGINE_API inline static const NxFr::StringId IdWidthButton = "WidthButton"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdWidthLabel = "WidthLabel"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdWidthInpuText = "WidthInpuText"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdRed = "Red"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdYellow = "Yellow"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdGreen = "Green"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdCyan = "Cyan"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdBlue = "Blue"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdMagenta = "Magenta"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdText_Normal = "Text_Normal"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdText_Title = "Text_Title"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdButton_Normal = "Button_Normal"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdButton_Pressed = "Button_Pressed"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdInfo = "Info"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdWarning = "Warning"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdError = "Error"_Sid;
			NEXUS_ENGINE_API inline static const NxFr::StringId IdFatal = "Fatal"_Sid;

			NEXUS_ENGINE_API static const Style Default;

			NEXUS_ENGINE_API static NxFr::Dictionary<NxFr::StringId, float>& GetVars();
			NEXUS_ENGINE_API static float& GetVar(NxFr::StringId Id);
			NEXUS_ENGINE_API static void RegisterVar(NxFr::StringId Id, float Instance);
			NEXUS_ENGINE_API static void UnregisterVar(NxFr::StringId Id);
			NEXUS_ENGINE_API static NxFr::Dictionary<NxFr::StringId, NxFr::Color>& GetColors();
			NEXUS_ENGINE_API static NxFr::Color& GetColor(NxFr::StringId Id);
			NEXUS_ENGINE_API static void RegisterColor(NxFr::StringId Id, NxFr::Color Instance);
			NEXUS_ENGINE_API static void UnregisterColor(NxFr::StringId Id);
			NEXUS_ENGINE_API static NxFr::Dictionary<NxFr::StringId, Style>& GetStyles();
			NEXUS_ENGINE_API static Style& GetStyle(NxFr::StringId Id);
			NEXUS_ENGINE_API static void RegisterStyle(NxFr::StringId Id, const Style& Instance);
			NEXUS_ENGINE_API static void UnregisterStyle(NxFr::StringId Id);

			NEXUS_ENGINE_API static Style Copy(const Style* Original);

			NEXUS_ENGINE_API Style();
			NEXUS_ENGINE_API ~Style();

			NEXUS_ENGINE_API void Reset();
			NEXUS_ENGINE_API void Push() const;
			NEXUS_ENGINE_API void Pop() const;
			NEXUS_ENGINE_API void SetPosition() const;
			NEXUS_ENGINE_API void SetWidth() const;
			NEXUS_ENGINE_API void SetWidthLabel(NxFr::StringView Label = "") const;

			Preset StylePreset;
			NxFr::Vector2f Position;
			float Width;
			float WidthLabel;
			float Height;
			NxFr::Color Color;
			NxFr::Color ColorText;
			NxFr::Color ColorBackground;
			NxFr::Color ColorBorder;
			float Alpha;
			NxFr::Vector2f Align;
			NxFr::Vector2f Spacing;
			NxFr::Vector2f Padding;
			float Rounding;
			float Border;
			float Font;
			uint64 Flag;
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
			node["StylePreset"] = (int32)rhs.StylePreset;
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
			rhs.StylePreset = (NxEn::GUI::Style::Preset)node["StylePreset"].as<int32>();
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
		out << YAML::Key << "StylePreset" << YAML::Value << (int32)rhs.StylePreset;
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
