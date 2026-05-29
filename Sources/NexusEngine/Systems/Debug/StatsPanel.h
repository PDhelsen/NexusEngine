#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

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

		void DrawButtons();
		void DrawFilter();
		void DrawStats(const NxFr::String& Label);
		bool FilterStats(const NxFr::String& Label);


	private:
		GUI::Style Style;

		NxFr::Instruments* Instruments;
		NxFr::Stats* Stats;
		NxFr::List<NxFr::String> Ids;
		NxFr::Dictionary<NxFr::StringId, const NxFr::Stats::Stat*> Values;
		NxFr::List<NxFr::StringView> Filters;
		NxFr::String Filter;
	};
}
