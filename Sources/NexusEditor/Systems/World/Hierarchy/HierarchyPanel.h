#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/TreePanel.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyPanel : public NxEn::TreePanel
	{
		friend class HierarchyManager;
		friend class HierarchyEditContext;

	public:
		NX_OBJECT(HierarchyPanel)

		HierarchyItem* GetItem(NxFr::GUID InstanceId) override;

		void SetManager(HierarchyManager* Manager, HierarchyEditContext* Context);

		void Find(NxFr::StringView Query) override;

	protected:
		void OnInitialize() override;
		void OnDraw() override;

		void OnSelectItem(NxFr::GUID InstanceId, bool State) override;

	private:
		EditSystem* Edit;

		HierarchyManager* Manager;
		HierarchyEditContext* Context;
	};
}
