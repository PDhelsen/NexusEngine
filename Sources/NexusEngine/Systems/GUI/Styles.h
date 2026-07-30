#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	namespace GUI
	{
		struct NX_ENGINE_API Style
		{
		public:
			static NxFr::Registry<float>& GetVars();
			static NxFr::Registry<NxFr::Color>& GetColors();
			static NxFr::Registry<Style>& GetStyles();

			static Style Copy(const Style* Original);
			static void Push(const Style* Instance);
			static void Pop(const Style* Instance);
			static void SetPosition(const Style* Instance);
			static void SetWidth(const Style* Instance);
			static void SetWidth(const Style* Instance, NxFr::StringView Text, bool Label = false);

			Style();
			~Style();
			void Reset();

			NxFr::Vector2f Position;
			NxFr::Vector2f Size;
			float Label;
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
