#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyEditContext.h"
#include "NexusEditor/Misc/Object/Inspector/InspectorPanel.h"
#include "NexusEditor/Misc/Object/Viewer/ViewerPanel.h"

namespace NxEd
{
	class StagePanel : public NxEn::GUI::Panel
	{
		friend class StagesSystem;

	public:
		NX_OBJECT(StagePanel)

		void Initialize(NxEn::ObjectInstance<NxEn::Object> Target);
		void Draw() override;

		void Select(NxFr::Handle<NxEn::GameObject> Instance);
		
		ViewerPanel* GetViewer() const { return Viewer; }
		InspectorPanel* GetInspector() const { return Inspector; }
		HierarchyPanel* GetHierarchy() const { return Hierarchy; }

		NxEn::ObjectInstance<NxEn::Object> GetTarget() const { return Target; }
		NxEn::World* GetWorld() const { return World; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

	private:
		void OnSelection(NxFr::GUID Id, bool State);
		bool AreElementsEnabled();

		NxEn::WorldSystem* Worlds;
		EditSystem* Edit;

		NxEn::GUI::Dock* Dock;
		ViewerPanel* Viewer;
		InspectorPanel* Inspector;
		HierarchyPanel* Hierarchy;
		HierarchyEditContext* Context;

		NxEn::ObjectInstance<NxEn::Object> Target;
		NxEn::World* World;
	};
}
