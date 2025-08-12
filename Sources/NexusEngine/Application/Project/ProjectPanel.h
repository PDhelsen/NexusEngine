#pragma once

#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class Project;

	class ProjectPanel : public GUI::Panel
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, ProjectPanel)

		NEXUS_ENGINE_API ProjectPanel();
		NEXUS_ENGINE_API ~ProjectPanel();

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;

		NEXUS_ENGINE_API void* FetchDefaultTarget() const override;

		Project* GetProject() const { return reinterpret_cast<Project*>(Target); }
	};
}
