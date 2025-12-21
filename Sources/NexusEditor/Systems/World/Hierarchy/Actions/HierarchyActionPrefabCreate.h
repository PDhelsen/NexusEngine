#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionPrefabCreate : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionPrefabCreate)

		NEXUS_EDITOR_API HierarchyActionPrefabCreate();
		NEXUS_EDITOR_API virtual ~HierarchyActionPrefabCreate();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) override;

	private:
		void Create(NxEn::WorldSystem* System, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items, NxFr::StringView Input) const;
		void Create(NxEn::WorldSystem* System, NxFr::Handle<NxEn::GameObject> Item, NxFr::StringView Input) const;
	};
}
