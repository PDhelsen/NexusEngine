#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class StatsPanel : public GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, StatsPanel)

		NEXUS_ENGINE_API StatsPanel();
		NEXUS_ENGINE_API ~StatsPanel();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnEnable() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

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
