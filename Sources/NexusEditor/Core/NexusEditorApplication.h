#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxEd
{
	NEXUS_APPLICATION_DECLARATION(NEXUS_EDITOR_API, ::NxEd::NexusEditorApplication)

	class NexusEditorApplication : public NxEn::NexusEngineApplication
	{
	public:
		static NexusEditorApplication* GetInstance() { return (NexusEditorApplication*)Application::GetInstance(); }

		NEXUS_EDITOR_API NexusEditorApplication();
		NEXUS_EDITOR_API virtual ~NexusEditorApplication();
		NEXUS_EDITOR_API virtual void Run() override;
	};
}
