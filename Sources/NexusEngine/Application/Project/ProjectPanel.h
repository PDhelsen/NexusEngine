#pragma once

#include "NexusEngine/Application/Project/Project.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class ProjectPanel : public GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, ProjectPanel)

		NEXUS_ENGINE_API ProjectPanel();
		NEXUS_ENGINE_API ~ProjectPanel();

		NEXUS_ENGINE_API void SetProject(const Project* Infos);
		NEXUS_ENGINE_API const Project* GetProject();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

	private:
		const Project* ProjectInfos;
	};
}
