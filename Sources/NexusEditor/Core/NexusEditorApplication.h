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

		NEXUS_EDITOR_API NexusEditorApplication(const NxEn::Project& ProjectInfo);
		NEXUS_EDITOR_API virtual ~NexusEditorApplication() = default;

	protected:
		NEXUS_EDITOR_API virtual void OnInitialize(NxEn::Bootstrapper& Bootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_EDITOR_API virtual void OnShutdown(NxEn::Bootstrapper& Unbootstrap, NxEn::SystemManager& Systems) override;
		NEXUS_EDITOR_API virtual void OnExecute(NxEn::Ticker& Ticks, NxEn::SystemManager& Systems) override;
	};
}
