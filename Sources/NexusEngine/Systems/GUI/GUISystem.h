#pragma once

#include "NexusEngine/Systems/System.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	class GUISystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, GUISystem)

		NEXUS_ENGINE_API GUISystem();
		NEXUS_ENGINE_API ~GUISystem();

		NEXUS_ENGINE_API void RegisterElement(GUI::Element* Element);
		NEXUS_ENGINE_API void UnregisterElement(GUI::Element* Element);

		NEXUS_ENGINE_API void LoadConfig(NxFr::StringView Name = "");
		NEXUS_ENGINE_API void SaveConfig(NxFr::StringView Name = "");
		NEXUS_ENGINE_API NxFr::Path GetConfigPath(NxFr::StringView Name = "");

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NxFr::Set<GUI::Element*> Elements;
	};
}
