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
		NEXUS_ENGINE_API static void UnregisterPanel(GUI::Panel* Instance);
		NEXUS_ENGINE_API static GUI::Menu* GetMenu();
		NEXUS_ENGINE_API static void RegisterMenuItem(GUI::Menu::Item* Instance);
		NEXUS_ENGINE_API static void UnregisterMenuItem(GUI::Menu::Item* Instance);

		NEXUS_ENGINE_API GUISystem();
		NEXUS_ENGINE_API ~GUISystem();

		NEXUS_ENGINE_API void RegisterElement(GUI::Element* Element);
		NEXUS_ENGINE_API void UnregisterElement(GUI::Element* Element);

		NEXUS_ENGINE_API void LoadLayout(NxFr::StringView Name = "");
		NEXUS_ENGINE_API void SaveLayout(NxFr::StringView Name = "");
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

		NEXUS_ENGINE_API void AddMenuWindowPanels() const;
		NEXUS_ENGINE_API void AddMenuWindowPanels(NxEn::GUI::Panel* Panel) const;
		NEXUS_ENGINE_API void AddMenuWindowLayouts() const;
		NEXUS_ENGINE_API void AddMenuWindowLayouts(const NxFr::String& Name) const;

		NEXUS_ENGINE_API NxFr::Path GetSettingsPath(NxFr::StringView Name, NxFr::StringView Default, NxFr::StringView Extension) const;
		NEXUS_ENGINE_API void LoadLayoutImGui(const NxFr::Path& Path) const;
		NEXUS_ENGINE_API void LoadLayoutNexus(const NxFr::Path& Path) const;
		NEXUS_ENGINE_API void SaveLayoutImGui(const NxFr::Path& Path) const;
		NEXUS_ENGINE_API void SaveLayoutNexus(const NxFr::Path& Path) const;
		NEXUS_ENGINE_API void LoadThemeImGui(const YAML::Node& Node) const;
		NEXUS_ENGINE_API void LoadThemeNexus(const YAML::Node& Node);
		NEXUS_ENGINE_API void SaveThemeImGui(YAML::Emitter& Emitter) const;
		NEXUS_ENGINE_API void SaveThemeNexus(YAML::Emitter& Emitter) const;

	private:
		NxFr::Set<GUI::Element*> Elements;
		NxFr::Dictionary<NxFr::StringId, GUI::Style> Styles;
	};
}
