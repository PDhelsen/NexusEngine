#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace YAML
{
	template<>
	struct convert<NxEn::GUI::Style>
	{
		static Node encode(const NxEn::GUI::Style& rhs)
		{
			Node node;
			node["Position"] = rhs.Position;
			node["Size"] = rhs.Size;
			node["Label"] = rhs.Label;
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
			rhs.Position = node["Position"].as<NxFr::Vector2f>();
			rhs.Size = node["Size"].as<NxFr::Vector2f>();
			rhs.Label = node["Label"].as<float>();
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
		out << YAML::Key << "Position" << YAML::Value << rhs.Position;
		out << YAML::Key << "Size" << YAML::Value << rhs.Size;
		out << YAML::Key << "Label" << YAML::Value << rhs.Label;
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
