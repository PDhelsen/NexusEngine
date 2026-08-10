#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Systems/GUI/Element.h"
#include "NexusEditor/Systems/Edit/EditSystem.h"
#include "NexusEngine/Misc/GUI/NodePanel.h"

namespace NxEd
{
	class NX_EDITOR_API ReferencesPanel : public NxEn::NodePanel
	{
	public:
		NX_OBJECT(ReferencesPanel)

		void Select(NxFr::GUID Id) override;

		bool IsLocked() const { return Lock; }
		void SetLocked(bool State) { Lock = State; }

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

	private:
		void DrawHeader();

		void FetchNodes(NxFr::GUID Id);
		void SortNodes(NxFr::GUID Id, int32 Depth = 0, bool From = true, bool To = true);
		void AlignNodes();

		void OnSelectionChanged(NxFr::GUID Id, bool State);

		NxEn::AssetsSystem* Assets;
		NxEn::WorldSystem* Worlds;
		EditSystem* Edit;

		bool Lock;
	};
}
