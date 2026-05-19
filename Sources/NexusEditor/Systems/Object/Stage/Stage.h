#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

#include "NexusEditor/Systems/Object/Viewer/ViewerPanel.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"
#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyPanel.h"

namespace NxEd
{
	class Stage : public NxEn::GUI::Element
	{
		friend class StageManager;

	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, Stage)

		NX_EDITOR_API Stage(NxEn::Object* Target);
		NX_EDITOR_API ~Stage();

		NX_EDITOR_API bool IsVisible() const;
		NX_EDITOR_API bool IsFocused() const;
		NX_EDITOR_API bool IsMain() const;

		NX_EDITOR_API NxEn::Object* GetTarget() const { return Target; }
		NX_EDITOR_API NxEn::World* GetWorld() const { return Viewer && Viewer->GetContext() ? Viewer->GetContext()->GetWorld() : nullptr;  }

		NX_EDITOR_API ViewerPanel* GetViewer() const { return Viewer; }
		NX_EDITOR_API InspectorPanel* GetInspector() const { return Inspector; }
		NX_EDITOR_API HierarchyPanel* GetHierarchy() const { return Hierarchy; }

	protected:
		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnShutdown() override;
		NX_EDITOR_API void OnEnable() override;
		NX_EDITOR_API void OnDisable() override;
		NX_EDITOR_API void OnGui(float TimeStep) override;

		NX_EDITOR_API void OnSelectionChanged(NxFr::GUID Id, bool State);

		NX_EDITOR_API void DrawDocking();
		NX_EDITOR_API void DockPanels();

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
