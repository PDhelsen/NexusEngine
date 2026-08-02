#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		NxFr::Registry<float>& Style::GetVars()
		{
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Registry<float> Vars;
			return Vars;
		}

		NxFr::Registry<NxFr::Color>& Style::GetColors()
		{
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Registry<NxFr::Color> Colors;
			return Colors;
		}

		NxFr::Registry<Style>& Style::GetStyles()
		{
			NxFr::Allocator::Scope Allocator(MemorySystem::GetAllocator(AllocatorType::General));

			static NxFr::Registry<Style> Styles;
			return Styles;
		}

		Style::Style()
			:
			Position(-NxFr::Vector2f::One), Size(NxFr::Vector2f::Zero), Label(0.0f),
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
