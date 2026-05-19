#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Styles.h"
#include "NexusEngine/Systems/GUI/Utils.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NX_ENGINE_API extern const NxFr::StringId GuiElementsId;
	}
}

namespace NxEn
{
	class GUISystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(NX_ENGINE_API, GUISystem)

		NX_ENGINE_API static GUI::Window* GetWindow();
		NX_ENGINE_API static GUI::Menu* GetMenu();
		NX_ENGINE_API static void RegisterMenuItem(GUI::Menu::Item* Instance);
		NX_ENGINE_API static void UnregisterMenuItem(GUI::Menu::Item* Instance);
		template<typename T> static T* GetPanel()
		{
			return (T*)GetPanel(T::GetClassType());
		}
		NX_ENGINE_API static GUI::Panel* GetPanel(NxFr::StringId Id);
		NX_ENGINE_API static void RegisterPanel(GUI::Panel* Instance);
		NX_ENGINE_API static void UnregisterPanel(GUI::Panel* Instance);

		NX_ENGINE_API GUISystem();
		NX_ENGINE_API ~GUISystem();

		NX_ENGINE_API void RegisterElement(GUI::Element* Element);
		NX_ENGINE_API void UnregisterElement(GUI::Element* Element);

		NX_ENGINE_API void LoadLayout(NxFr::StringView Name = "");
		NX_ENGINE_API void SaveLayout(NxFr::StringView Name = "");
		NX_ENGINE_API void LoadTheme(NxFr::StringView Name = "");
		NX_ENGINE_API void SaveTheme(NxFr::StringView Name = "");

		NX_ENGINE_API GUI::Panel* GetActivePanel() const;

	protected:
		NX_ENGINE_API void OnInitialize() override;
		NX_ENGINE_API void OnShutdown() override;
		NX_ENGINE_API void OnTick(float TimeStep = 0.0f) override;

	private:
		NX_ENGINE_API void AddMenuWindowPanels() const;
		NX_ENGINE_API void AddMenuWindowPanels(NxEn::GUI::Panel* Panel) const;
		NX_ENGINE_API void AddMenuWindowLayouts() const;
		NX_ENGINE_API void AddMenuWindowLayouts(const NxFr::String& Name) const;

		NX_ENGINE_API void LoadLayoutImGui(const NxFr::String& Path) const;
		NX_ENGINE_API void LoadLayoutNexus(const NxFr::String& Path) const;
		NX_ENGINE_API void SaveLayoutImGui(const NxFr::String& Path) const;
		NX_ENGINE_API void SaveLayoutNexus(const NxFr::String& Path) const;
		NX_ENGINE_API void LoadThemeImGui(const YAML::Node& Node) const;
		NX_ENGINE_API void LoadThemeNexus(const YAML::Node& Node);
		NX_ENGINE_API void SaveThemeImGui(YAML::Emitter& Emitter) const;
		NX_ENGINE_API void SaveThemeNexus(YAML::Emitter& Emitter) const;

	private:
		NxFr::Set<GUI::Element*> Elements;
	};
}
