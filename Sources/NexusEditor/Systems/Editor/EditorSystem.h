#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"
#include "NexusEditor/Systems/Object/Stage/StageManager.h"

namespace NxEd
{
	class NX_EDITOR_API EditorSystem : public NxEn::System
	{
	public:
		NX_OBJECT_DECLARATION(EditorSystem)

		EditorSystem();
		~EditorSystem();

		void SaveAll();

		NxFr::Event<>& GetOnSave() { return OnSave; }

		NxEn::Input::Schema& GetInputsSchema() { return InputSchema; }
		NxEn::GUI::Window& GetWindow() { return *Window; }
		AssetsBrowser& GetAssetsBrowser() { return *Browser; }
		HierarchyManager& GetHierarchyManager() { return *Hierarchy; }
		StageManager& GetStageManager() { return *Stages; }


	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

		void ApplySettings();

	private:
		NxFr::Event<> OnSave;

		NxEn::Input::Schema InputSchema;
		NxEn::GUI::Window* Window;

		AssetsBrowser* Browser;
		HierarchyManager* Hierarchy;
		StageManager* Stages;
	};
}
