#pragma once

#include "NexusSandbox-Editor/Core/NexusSandboxEditorCore.h"
#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxSE
{
	NX_APPLICATION_DECLARATION(NX_SANDBOX_EDITOR_API, ::NxSE::NexusSandboxEditorApplication)

	class NX_SANDBOX_EDITOR_API NexusSandboxEditorApplication : public NxEd::NexusEditorApplication
	{
	public:
		NexusSandboxEditorApplication(const NxEn::Project& ProjectInfo);
		virtual ~NexusSandboxEditorApplication();

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnRun() override;
	};
}
