#pragma once

#include "NexusSandbox-Editor/Core/NexusSandboxEditorCore.h"
#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxSE
{
	NX_APPLICATION_DECLARATION(NX_SANDBOX_EDITOR_API, ::NxSE::NexusSandboxEditorApplication)

	class NexusSandboxEditorApplication : public NxEd::NexusEditorApplication
	{
	public:
		NX_SANDBOX_EDITOR_API NexusSandboxEditorApplication(const NxEn::Project& ProjectInfo);
		NX_SANDBOX_EDITOR_API virtual ~NexusSandboxEditorApplication();

	protected:
		NX_SANDBOX_EDITOR_API virtual void OnInitialize() override;
		NX_SANDBOX_EDITOR_API virtual void OnShutdown() override;
		NX_SANDBOX_EDITOR_API virtual void OnExecute() override;
	};
}
