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

		NEXUS_ENGINE_API void SetStats(const NxFr::Stats* Stats);
		NEXUS_ENGINE_API const NxFr::Stats* GetStats();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

		void DrawFilter();
		void DrawStats(const NxFr::String& Label) const;
		bool FilterStats(const NxFr::String& Label);

	private:
		const NxFr::Stats* Statistiques;
		NxFr::List<const NxFr::String*> Ids;
		NxFr::Dictionary<NxFr::StringId, const NxFr::Stats::Stat*> Values;
		NxFr::List<NxFr::StringView> Filters;
	};
}
