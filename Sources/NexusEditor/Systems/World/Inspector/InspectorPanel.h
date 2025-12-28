#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class InspectorPanel : public NxEn::GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, InspectorPanel)
			
		NEXUS_EDITOR_API void Show(NxFr::Handle<NxEn::GameObject> Instance);

	protected:
		NEXUS_EDITOR_API void OnGui(float TimeStep) override;

	private:
		NxFr::Handle<NxEn::GameObject> Target;
	};
}
