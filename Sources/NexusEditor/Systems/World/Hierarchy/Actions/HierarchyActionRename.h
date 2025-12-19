#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionRename : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionRename)

		NEXUS_EDITOR_API HierarchyActionRename();
		NEXUS_EDITOR_API virtual ~HierarchyActionRename();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) override;

	private:
		void Rename(NxEn::World* World, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items, NxFr::StringView Input) const;
		void Rename(NxEn::World* World, NxFr::Handle<NxEn::GameObject> Item, NxFr::StringView Input) const;
	};
}
