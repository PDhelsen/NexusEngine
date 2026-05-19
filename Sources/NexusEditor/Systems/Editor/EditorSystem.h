#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"
#include "NexusEditor/Systems/Object/Stage/StageManager.h"

namespace NxEd
{
	class EditorSystem : public NxEn::System
	{
	public:
		NX_OBJECT_DECLARATION(NX_EDITOR_API, EditorSystem)

		NX_EDITOR_API EditorSystem();
		NX_EDITOR_API ~EditorSystem();

		NX_EDITOR_API void SaveAll();

		NX_EDITOR_API NxFr::Event<>& GetOnSave() { return OnSave; }

		NX_EDITOR_API NxEn::Input::Schema& GetInputsSchema() { return InputSchema; }
		NX_EDITOR_API NxEn::GUI::Window& GetWindow() { return *Window; }
		NX_EDITOR_API AssetsBrowser& GetAssetsBrowser() { return *Browser; }
		NX_EDITOR_API HierarchyManager& GetHierarchyManager() { return *Hierarchy; }
		NX_EDITOR_API StageManager& GetStageManager() { return *Stages; }


	protected:
		NX_EDITOR_API void OnInitialize() override;
		NX_EDITOR_API void OnShutdown() override;
		NX_EDITOR_API void OnTick(float TimeStep = 0.0f) override;

		NX_EDITOR_API void ApplySettings();

	private:
		NxFr::Event<> OnSave;

		NxEn::Input::Schema InputSchema;
		NxEn::GUI::Window* Window;

		AssetsBrowser* Browser;
		HierarchyManager* Hierarchy;
		StageManager* Stages;
	};
}
