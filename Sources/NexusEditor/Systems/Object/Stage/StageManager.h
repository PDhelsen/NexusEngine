#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEditor/Systems/Object/Stage/Stage.h"

namespace NxEd
{
	class NX_EDITOR_API StageManager
	{
	public:
		StageManager();
		~StageManager();

		void Tick(float TimeStep);

		Stage* CreateStage(NxEn::Object* Target);
		void DestroyStage(NxEn::Object* Target);
		void ShowStage(NxEn::Object* Target);

		Stage* GetStage(NxEn::Object* Target) const;
		Stage* GetMainStage() const;
		Stage* GetFocusedStage() const;

	private:
		NxFr::Dictionary<NxEn::Object*, Stage*> Stages;
	};
}
