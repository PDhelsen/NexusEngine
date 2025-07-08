#pragma once

#include "NexusSandbox-Editor/Core/NexusSandboxEditorCore.h"
#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxSE
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_SANDBOX_EDITOR_API, ::NxSE::NexusSandboxEditorApplication)

	class NexusSandboxEditorApplication : public NxEd::NexusEditorApplication
	{
	public:
		static NexusSandboxEditorApplication* GetInstance() { return (NexusSandboxEditorApplication*)Application::GetInstance(); }

		NEXUS_SANDBOX_EDITOR_API NexusSandboxEditorApplication(const NxEn::Project& ProjectInfo);
		NEXUS_SANDBOX_EDITOR_API virtual ~NexusSandboxEditorApplication() = default;

	protected:
		NEXUS_SANDBOX_EDITOR_API virtual void OnInitialize() override;
		NEXUS_SANDBOX_EDITOR_API virtual void OnShutdown() override;
		NEXUS_SANDBOX_EDITOR_API virtual void OnExecute() override;
	};
}
