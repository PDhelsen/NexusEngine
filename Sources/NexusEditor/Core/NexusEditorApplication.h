#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/World/Hierarchy/HierarchyManager.h"

namespace NxEd
{
	NX_APPLICATION_DECLARATION(NX_EDITOR_API, ::NxEd::NexusEditorApplication)

	class NX_EDITOR_API NexusEditorApplication : public NxEn::NexusEngineApplication
	{
	public:
		NexusEditorApplication(const NxEn::Project& ProjectInfo);
		virtual ~NexusEditorApplication();

		void SaveAll();

		NxFr::Event<>& GetOnSave() { return OnSave; }

		NxEn::Input::Schema* GetInputsEditor() { return Inputs; }
		AssetsBrowser* GetAssetsBrowser() { return Browser; }
		HierarchyManager* GetHierarchyManager() { return Hierarchy; }

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnRun() override;

	private:
		void ApplyShortcuts();

		NxFr::Event<> OnSave;

		NxEn::Input::Schema* Inputs;
		AssetsBrowser* Browser;
		HierarchyManager* Hierarchy;
	};
}
