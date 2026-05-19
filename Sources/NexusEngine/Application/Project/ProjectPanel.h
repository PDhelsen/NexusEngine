#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class Project;

	class ProjectPanel : public GUI::Panel
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, ProjectPanel)

		NX_ENGINE_API ProjectPanel();
		NX_ENGINE_API ~ProjectPanel();

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnEnable() override;
		NX_ENGINE_API void OnGui(float TimeStep) override;

	private:
		GUI::Style Style;

		Project* Infos;
	};
}
