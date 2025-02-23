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

		NEXUS_SANDBOX_EDITOR_API NexusSandboxEditorApplication() = default;
		NEXUS_SANDBOX_EDITOR_API virtual ~NexusSandboxEditorApplication() = default;

	protected:
		NEXUS_SANDBOX_EDITOR_API virtual void Initialize(NxEn::Bootstrapper& Bootstrap) override;
		NEXUS_SANDBOX_EDITOR_API virtual void Shutdown(NxEn::Bootstrapper& Bootstrap) override;
	};
}
