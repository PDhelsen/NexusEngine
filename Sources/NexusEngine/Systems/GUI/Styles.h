#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace GUI
	{
		struct Style
		{
		public:
			enum class Preset
			{
				Text, Button, Panel
			};

			struct Scope
			{
			public:
				NEXUS_ENGINE_API Scope(NxFr::StringId Id);
				NEXUS_ENGINE_API ~Scope();

			private:
				NxFr::StringId Id;
			};

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

			NEXUS_ENGINE_API Style() = default;
			NEXUS_ENGINE_API ~Style() = default;

			NEXUS_ENGINE_API void Reset();
			NEXUS_ENGINE_API void Push();
			NEXUS_ENGINE_API void Pop();

			Preset StylePreset;
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
			return node;
		}

		static bool decode(const Node& node, NxEn::GUI::Style& rhs)
		{
			rhs.StylePreset = (NxEn::GUI::Style::Preset)node["StylePreset"].as<int32>();
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
			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxEn::GUI::Style& rhs)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "StylePreset" << YAML::Value << (int32)rhs.StylePreset;
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
		out << YAML::EndMap;
		return out;
	}
}
