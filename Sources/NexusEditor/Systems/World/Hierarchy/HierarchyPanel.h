#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/TreePanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyItem.h"

namespace NxEd
{
	class NX_EDITOR_API HierarchyPanel : public NxEn::Rework::TreePanel
	{
		friend class HierarchyManager;

	public:
		NX_OBJECT(HierarchyPanel)

		HierarchyItem* GetItem(NxFr::GUID Id) override;

		void SetManager(HierarchyManager* Manager);

		void Find(NxFr::StringView Query) override;

	protected:
		void OnInitialize() override;
		void OnDraw() override;

		void OnSelectItem(NxFr::GUID Id, bool State) override;

	private:
		HierarchyManager* Manager;
	};
}
