#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Project/Project.h"
#include "NexusEngine/Systems/GUI/Elements/Panel.h"

namespace NxEn
{
	class NX_ENGINE_API ProjectPanel : public GUI::Panel
	{
	public:
		NX_OBJECT(ProjectPanel)

		ProjectPanel();
		~ProjectPanel();

	protected:
		void OnInitialize() override;
		void OnEnable() override;
		void OnDraw() override;

	private:
		Project* Infos;
	};
}
