#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace GUI
	{
		struct NX_ENGINE_API Style
		{
		public:
			enum class Type
			{
				Text, Button, Panel
			};

			struct NX_ENGINE_API Scope
			{
			public:
				Scope(const Style* Instance);
				~Scope();

			private:
				const Style* Instance;
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

			Type StyleType;
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

NX_ENUM_STRING(NxEn::GUI::Style::Type, 3, "Text", "Button", "Panel")
