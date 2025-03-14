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

	protected:
		NEXUS_EDITOR_API virtual void OnInitialize(NxEn::Bootstrapper& Bootstrap) override;
		NEXUS_EDITOR_API virtual void OnShutdown(NxEn::Bootstrapper& Unbootstrap) override;
	};
}
