#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEditor/Systems/Stages/StagePanel.h"
#include "NexusEngine/Misc/Object/ObjectInstance.h"

namespace NxEd
{
	class StagesSystem : public NxEn::System
	{
	public:
		NX_OBJECT(StagesSystem)

		StagePanel* CreateStage(NxEn::ObjectInstance<NxEn::Object> Instance);
		void DestroyStage(NxFr::GUID Id);

		StagePanel* GetStage(NxFr::GUID Id);
		StagePanel* GetFocusedStage();
		StagePanel* GetMainStage();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep) override;

	private:
		NxFr::Dictionary<NxFr::GUID, StagePanel*> Stages;
		StagePanel* MainStage;
	};
}
