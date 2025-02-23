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

		NEXUS_EDITOR_API NexusEditorApplication() = default;
		NEXUS_EDITOR_API virtual ~NexusEditorApplication() = default;

	protected:
		NEXUS_EDITOR_API virtual void Initialize(NxEn::Bootstrapper& Bootstrap) override;
		NEXUS_EDITOR_API virtual void Shutdown(NxEn::Bootstrapper& Bootstrap) override;
	};
}
