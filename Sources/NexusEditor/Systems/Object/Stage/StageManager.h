#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Object/Stage/Stage.h"

namespace NxEd
{
	class StageManager
	{
	public:
		NEXUS_EDITOR_API StageManager();
		NEXUS_EDITOR_API ~StageManager();

		NEXUS_EDITOR_API Stage* CreateStage(NxEn::Object* Target);
		NEXUS_EDITOR_API void ShowStage(NxEn::Object* Target);
		NEXUS_EDITOR_API void DestroyStage(NxEn::Object* Target);
		NEXUS_EDITOR_API void DestroyDisableStage();

		NEXUS_EDITOR_API Stage* GetStage(NxEn::Object* Target) const;
		NEXUS_EDITOR_API Stage* GetMainStage() const;
		NEXUS_EDITOR_API Stage* GetFocusedStage() const;

	private:
		NxFr::Dictionary<NxEn::Object*, Stage*> Stages;
	};
}
