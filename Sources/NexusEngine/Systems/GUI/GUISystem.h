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
		NX_OBJECT(GUISystem)

		template<typename T> static T* GetPanel() { return static_cast<T*>(GetPanels().TryGet(T::GetClassType())); }

		static NxFr::Registry<GUI::Panel*>& GetPanels();
		static NxFr::Registry<GUI::Menu::Item>& GetMenuItems();

		GUISystem();
		~GUISystem();

		void RegisterElement(GUI::Element* Element);
		void UnregisterElement(GUI::Element* Element);

		void LoadLayout(NxFr::StringView Name = "");
		void SaveLayout(NxFr::StringView Name = "");
		void LoadTheme(NxFr::StringView Name = "");
		void SaveTheme(NxFr::StringView Name = "");

		GUI::Element* GetElement(NxFr::StringView Name, NxFr::StringView Id) const;
		GUI::Panel* GetActivePanel() const;
		GUI::Window* GetWindow();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void AddMenuWindowItems();
		void AddMenuWindowItems(const GUI::Menu::Item& Item);
		void AddMenuWindowPanels();
		void AddMenuWindowPanels(NxEn::GUI::Panel* Panel);
		void AddMenuWindowLayouts();
		void AddMenuWindowLayouts(const NxFr::String& Name);

		void LoadLayoutImGui(const NxFr::String& Path) const;
		void LoadLayoutNexus(const NxFr::String& Path) const;
		void SaveLayoutImGui(const NxFr::String& Path) const;
		void SaveLayoutNexus(const NxFr::String& Path) const;
		void LoadThemeImGui(const YAML::Node& Node) const;
		void LoadThemeNexus(const YAML::Node& Node);
		void SaveThemeImGui(YAML::Emitter& Emitter) const;
		void SaveThemeNexus(YAML::Emitter& Emitter) const;

	private:
		GUI::Window Window;
		NxFr::Set<GUI::Element*> Elements;
	};
}
