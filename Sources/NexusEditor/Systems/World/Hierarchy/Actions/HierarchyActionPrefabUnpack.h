#pragma once

#include "NexusEditor/Systems/World/Hierarchy/HierarchyAction.h"

namespace NxEd
{
	class HierarchyActionPrefabUnpack : public HierarchyAction
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyActionPrefabUnpack)

		NEXUS_EDITOR_API HierarchyActionPrefabUnpack();
		NEXUS_EDITOR_API virtual ~HierarchyActionPrefabUnpack();

		NEXUS_EDITOR_API void Execute(const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) override;

	private:
		void Unpack(NxEn::WorldSystem* System, const NxFr::Array<NxFr::Handle<NxEn::GameObject>>& Items) const;
		void Unpack(NxEn::WorldSystem* System, NxFr::Handle<NxEn::GameObject> Item) const;
	};
}
