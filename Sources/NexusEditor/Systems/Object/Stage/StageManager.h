#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Object/Stage/Stage.h"

namespace NxEd
{
	class StageManager
	{
	public:
		NX_EDITOR_API StageManager();
		NX_EDITOR_API ~StageManager();

		NX_EDITOR_API Stage* CreateStage(NxEn::Object* Target);
		NX_EDITOR_API void ShowStage(NxEn::Object* Target);
		NX_EDITOR_API void DestroyStage(NxEn::Object* Target);
		NX_EDITOR_API void DestroyDisableStage();

		NX_EDITOR_API Stage* GetStage(NxEn::Object* Target) const;
		NX_EDITOR_API Stage* GetMainStage() const;
		NX_EDITOR_API Stage* GetFocusedStage() const;

	private:
		NxFr::Dictionary<NxEn::Object*, Stage*> Stages;
	};
}
