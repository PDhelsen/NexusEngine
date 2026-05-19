#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxEd
{
	NX_APPLICATION_DECLARATION(NX_EDITOR_API, ::NxEd::NexusEditorApplication)

	class NexusEditorApplication : public NxEn::NexusEngineApplication
	{
	public:
		NX_EDITOR_API NexusEditorApplication(const NxEn::Project& ProjectInfo);
		NX_EDITOR_API virtual ~NexusEditorApplication();

	protected:
		NX_EDITOR_API virtual void OnInitialize() override;
		NX_EDITOR_API virtual void OnShutdown() override;
		NX_EDITOR_API virtual void OnExecute() override;
	};
}
