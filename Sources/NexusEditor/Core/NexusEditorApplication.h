#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_EDITOR_API, ::NxEd::NexusEditorApplication)

	class NexusEditorApplication : public NxEn::NexusEngineApplication
	{
	public:
		NEXUS_EDITOR_API NexusEditorApplication(const NxEn::Project& ProjectInfo);
		NEXUS_EDITOR_API virtual ~NexusEditorApplication();

	protected:
		NEXUS_EDITOR_API virtual void OnInitialize() override;
		NEXUS_EDITOR_API virtual void OnShutdown() override;
		NEXUS_EDITOR_API virtual void OnExecute() override;
	};
}
