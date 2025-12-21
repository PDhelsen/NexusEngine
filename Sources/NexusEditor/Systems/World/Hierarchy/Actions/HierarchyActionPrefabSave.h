#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionPrefabSave : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionPrefabSave)

		NEXUS_EDITOR_API HierarchyActionPrefabSave();
		NEXUS_EDITOR_API virtual ~HierarchyActionPrefabSave();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) override;

	private:
		void Save(NxEn::WorldSystem* System, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const;
		void Save(NxEn::WorldSystem* System, NxFr::Handle<NxEn::GameObject> Item) const;
	};
}
