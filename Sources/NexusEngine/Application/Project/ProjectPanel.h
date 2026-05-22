#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Project/Project.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class NX_ENGINE_API ProjectPanel : public GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(ProjectPanel)

		ProjectPanel();
		~ProjectPanel();

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnGui(float TimeStep) override;

	private:
		GUI::Style Style;

		Project* Infos;
	};
}
