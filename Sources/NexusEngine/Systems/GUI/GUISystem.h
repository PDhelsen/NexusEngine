#pragma once

#include "NexusEngine/Systems/System.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	class GUISystem : public System
	{
	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, GUISystem)

		template<typename T>
		static T* GetPanel()
		{
			return (T*)GetPanel(T::GetClassType());
		}
		NEXUS_ENGINE_API static GUI::Panel* GetPanel(NxFr::StringId Id);
		NEXUS_ENGINE_API static void RegisterPanel(GUI::Panel* Instance);
		NEXUS_ENGINE_API static GUI::Menu* GetMenu();
		NEXUS_ENGINE_API static void RegisterMenuItem(const GUI::Menu::Item* Instance);

		NEXUS_ENGINE_API GUISystem();
		NEXUS_ENGINE_API ~GUISystem();

		NEXUS_ENGINE_API void RegisterElement(GUI::Element* Element);
		NEXUS_ENGINE_API void UnregisterElement(GUI::Element* Element);

		NEXUS_ENGINE_API void LoadConfig(NxFr::StringView Name = "");
		NEXUS_ENGINE_API void SaveConfig(NxFr::StringView Name = "");
		NEXUS_ENGINE_API void LoadTheme(NxFr::StringView Name = "");
		NEXUS_ENGINE_API void SaveTheme(NxFr::StringView Name = "");

		NEXUS_ENGINE_API void AppendStyle(NxFr::StringId Id, const GUI::Style& Style);
		NEXUS_ENGINE_API void AssignStyle(NxFr::StringId Id, const GUI::Style& Style);
		NEXUS_ENGINE_API void RemoveStyle(NxFr::StringId Id);
		NEXUS_ENGINE_API const GUI::Style& GetStyle(NxFr::StringId Id);

	protected:
		NEXUS_ENGINE_API void OnInitialize() override;
		NEXUS_ENGINE_API void OnShutdown() override;
		NEXUS_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

		NEXUS_ENGINE_API NxFr::Path GetSettingsPath(NxFr::StringView Name, NxFr::StringView Saved, NxFr::StringView Extension);
		NEXUS_ENGINE_API void LoadThemeImGui(const YAML::Node& Node);
		NEXUS_ENGINE_API void LoadThemeNexus(const YAML::Node& Node);
		NEXUS_ENGINE_API void SaveThemeImGui(YAML::Emitter& Emitter);
		NEXUS_ENGINE_API void SaveThemeNexus(YAML::Emitter& Emitter);

	private:
		NxFr::Set<GUI::Element*> Elements;
		NxFr::Dictionary<NxFr::StringId, GUI::Style> Styles;
	};
}
