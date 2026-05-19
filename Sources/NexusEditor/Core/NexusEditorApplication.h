#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Core/NexusEngineApplication.h"

namespace NxEd
{
	NX_APPLICATION_DECLARATION(NX_EDITOR_API, ::NxEd::NexusEditorApplication)

	class NX_EDITOR_API NexusEditorApplication : public NxEn::NexusEngineApplication
	{
	public:
		NexusEditorApplication(const NxEn::Project& ProjectInfo);
		virtual ~NexusEditorApplication();

	protected:
		virtual void OnInitialize() override;
		virtual void OnShutdown() override;
		virtual void OnExecute() override;
	};
}
