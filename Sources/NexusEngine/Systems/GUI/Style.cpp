#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Style.h"

namespace NxEn
{
	namespace GUI
	{
		Transform::Transform()
			: Position(-NxFr::Vector2f::One), Size(-NxFr::Vector2f::One), Label(Styles::WidthLabel())
		{
		}

		Transform::Transform(NxFr::Vector2f Position, NxFr::Vector2f Size, float Label)
			: Position(Position), Size(Size), Label(Label)
		{
		}

		Transform::~Transform()
		{
		}

		Style::Scope::Scope(const Style* Visual)
			: Visual(Visual)
		{
			Utils::PushStyle(Visual);
		}

		Style::Scope::~Scope()
		{
			Utils::PopStyle(Visual);
		}

		NxFr::Registry<float>& Style::GetVars()
		{
			static NxFr::Registry<float> Vars;
			return Vars;
		}

		NxFr::Registry<NxFr::Color>& Style::GetColors()
		{
			static NxFr::Registry<NxFr::Color> Colors;
			return Colors;
		}

		NxFr::Registry<Style>& Style::GetStyles()
		{
			static NxFr::Registry<Style> Styles;
			return Styles;
		}

		Style::Style()
			:
			Color(NxFr::Colors::White), ColorText(NxFr::Colors::White), ColorBackground(NxFr::Colors::Black), ColorBorder(NxFr::Colors::Gray), Alpha(1.0f),
			Align(NxFr::Vector2f(0.5f, 0.5f)), Spacing(NxFr::Vector2f(8.0f, 4.0f)), Padding(NxFr::Vector2f(4.0f , 3.0f)), Rounding(0.0f), Border(0.0f),
			Font(1.0f),
			Flag(0)
		{
		}

		Style::~Style()
		{
		}

		void Style::Reset()
		{
			*this = GUI::Styles::Default();
		}
	}
}
