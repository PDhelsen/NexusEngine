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
		NX_OBJECT(Stage)

		Stage(NxEn::Object* Target);
		~Stage();

		bool IsVisible() const;
		bool IsFocused() const override;
		bool IsMain() const;

		NxEn::Object* GetTarget() const { return Target; }
		NxEn::World* GetWorld() const { return World; }

		ViewerPanel* GetViewer() const { return Viewer; }
		InspectorPanel* GetInspector() const { return Inspector; }
		HierarchyPanel* GetHierarchy() const { return Hierarchy; }

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnDraw() override;

		void OnSelectionChanged(NxFr::GUID Id, bool State);

	private:
		NxEn::Object* Target;
		NxEn::World* World;

		NxEn::GUI::Dock Dock;
		bool Main;

		ViewerPanel* Viewer;
		InspectorPanel* Inspector;
		HierarchyPanel* Hierarchy;
	};
}
