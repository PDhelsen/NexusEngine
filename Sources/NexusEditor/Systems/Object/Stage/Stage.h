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
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, Stage)

		NEXUS_EDITOR_API Stage(NxEn::Object* Target);
		NEXUS_EDITOR_API ~Stage();

		NEXUS_EDITOR_API bool IsVisible() const;
		NEXUS_EDITOR_API bool IsFocused() const;
		NEXUS_EDITOR_API bool IsMain() const;

		NEXUS_EDITOR_API NxEn::Object* GetTarget() const { return Target; }
		NEXUS_EDITOR_API NxEn::World* GetWorld() const { return Viewer && Viewer->GetContext() ? Viewer->GetContext()->GetWorld() : nullptr;  }

		NEXUS_EDITOR_API ViewerPanel* GetViewer() const { return Viewer; }
		NEXUS_EDITOR_API InspectorPanel* GetInspector() const { return Inspector; }
		NEXUS_EDITOR_API HierarchyPanel* GetHierarchy() const { return Hierarchy; }

	protected:
		NEXUS_EDITOR_API void OnInitialize() override;
		NEXUS_EDITOR_API void OnShutdown() override;
		NEXUS_EDITOR_API void OnEnable() override;
		NEXUS_EDITOR_API void OnDisable() override;
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

		NEXUS_EDITOR_API void DrawDocking();
		NEXUS_EDITOR_API void DockPanels();

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
