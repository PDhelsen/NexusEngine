#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

#include "NexusEditor/Systems/Object/Viewer/ViewerPanel.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"
#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class NX_EDITOR_API Stage : public NxEn::GUI::Element
	{
		friend class StageManager;

	public:
		NX_OBJECT_DECLARATION(Stage)

		Stage(NxEn::Object* Target);
		~Stage();

		bool IsVisible() const;
		bool IsFocused() const;
		bool IsMain() const;

		NxEn::Object* GetTarget() const { return Target; }
		NxEn::World* GetWorld() const { return Viewer && Viewer->GetContext() ? Viewer->GetContext()->GetWorld() : nullptr;  }

		ViewerPanel* GetViewer() const { return Viewer; }
		InspectorPanel* GetInspector() const { return Inspector; }
		HierarchyPanel* GetHierarchy() const { return Hierarchy; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnGui(float TimeStep) override;

		void OnSelectionChanged(NxFr::GUID Id, bool State);

		void DrawDocking();
		void DockPanels();

	private:
		NxEn::Object* Target;

		uint32 DockId;
		bool Layout;
		bool Main;

		ViewerPanel* Viewer;
		InspectorPanel* Inspector;
		HierarchyPanel* Hierarchy;
	};
}
