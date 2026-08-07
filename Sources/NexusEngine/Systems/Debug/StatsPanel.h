#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Element.h"
#include "NexusEngine/Misc/Utils/Filter.h"

namespace NxEn
{
	class NX_ENGINE_API StatsPanel : public GUI::Panel
	{
	public:
		NX_OBJECT(StatsPanel)

		StatsPanel();
		~StatsPanel();

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDraw() override;

	private:
		void DrawButtons();
		void DrawFilter();
		void DrawStats(NxFr::StringView Label);
		bool FilterStats(NxFr::StringView Label);

		NxFr::Instruments* Instruments;
		NxFr::Stats* Stats;
		NxFr::List<NxFr::String> Ids;
		NxFr::Dictionary<NxFr::StringId, const NxFr::Stats::Stat*> Values;
		Utils::Filter Filter;
	};
}
