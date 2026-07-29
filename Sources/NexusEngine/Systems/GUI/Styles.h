#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace GUI
	{
		struct Style;

		struct NX_ENGINE_API Scope
		{
		public:
			Scope(NxFr::StringId Id);
			Scope(const Style* Instance);
			~Scope();

		private:
			NxFr::StringId Id;
			const Style* Instance;
		};

		struct NX_ENGINE_API Style
		{
		public:
			enum class Preset
			{
				Text, Button, Panel
			};

			static NxFr::Registry<float>& GetVars();
			static NxFr::Registry<NxFr::Color>& GetColors();
			static NxFr::Registry<Style>& GetStyles();

			static Style Copy(const Style* Original);

			Style();
			~Style();

			void Reset();
			void Push() const;
			void Pop() const;
			void SetPosition() const;
			void SetWidth() const;
			void SetWidthLabel(NxFr::StringView Label = "") const;

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
