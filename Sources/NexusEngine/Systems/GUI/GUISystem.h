#pragma once

#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Styles.h"
#include "NexusEngine/Systems/GUI/Utils.h"

namespace NxEn
{
	class NX_ENGINE_API GUISystem : public System
	{
	public:
		NX_OBJECT_DECLARATION(GUISystem)

		static GUI::Window* GetWindow();
		static GUI::Menu* GetMenu();
		static void RegisterMenuItem(GUI::Menu::Item* Instance);
		static void UnregisterMenuItem(GUI::Menu::Item* Instance);
		template<typename T> static T* GetPanel()
		{
			return (T*)GetPanel(T::GetClassType());
		}
		static GUI::Panel* GetPanel(NxFr::StringId Id);
		static void RegisterPanel(GUI::Panel* Instance);
		static void UnregisterPanel(GUI::Panel* Instance);

		GUISystem();
		~GUISystem();

		void RegisterElement(GUI::Element* Element);
		void UnregisterElement(GUI::Element* Element);

		void LoadLayout(NxFr::StringView Name = "");
		void SaveLayout(NxFr::StringView Name = "");
		void LoadTheme(NxFr::StringView Name = "");
		void SaveTheme(NxFr::StringView Name = "");

		GUI::Panel* GetActivePanel() const;

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void AddMenuWindowPanels() const;
		void AddMenuWindowPanels(NxEn::GUI::Panel* Panel) const;
		void AddMenuWindowLayouts() const;
		void AddMenuWindowLayouts(const NxFr::String& Name) const;

		void LoadLayoutImGui(const NxFr::String& Path) const;
		void LoadLayoutNexus(const NxFr::String& Path) const;
		void SaveLayoutImGui(const NxFr::String& Path) const;
		void SaveLayoutNexus(const NxFr::String& Path) const;
		void LoadThemeImGui(const YAML::Node& Node) const;
		void LoadThemeNexus(const YAML::Node& Node);
		void SaveThemeImGui(YAML::Emitter& Emitter) const;
		void SaveThemeNexus(YAML::Emitter& Emitter) const;

	private:
		NxFr::Set<GUI::Element*> Elements;
	};
}
