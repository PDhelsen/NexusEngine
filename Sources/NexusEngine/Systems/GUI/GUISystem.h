#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/GUI/GUI.h"
#include "NexusEngine/Systems/GUI/Styles.h"
#include "NexusEngine/Systems/GUI/Utils.h"
#include "NexusEngine/Systems/GUI/Misc/Constants.h"

namespace NxEn
{
	class NX_ENGINE_API GUISystem : public System
	{
	public:
		NX_OBJECT(GUISystem)

		static NxFr::Registry<GUI::Panel*>& GetPanels();
		static NxFr::Registry<GUI::Menu::Item>& GetMenuItems();

		static NxFr::StringId ImGuiToNexusId(NxFr::StringView Name);
		static NxFr::String NexusToImGuiId(NxFr::StringView Name, NxFr::StringView Id);

		GUISystem();
		~GUISystem();

		void RegisterElement(GUI::Element* Element);
		void UnregisterElement(GUI::Element* Element);

		void LoadLayout(NxFr::StringView Name = "");
		void SaveLayout(NxFr::StringView Name = "");
		void LoadTheme(NxFr::StringView Name = "");
		void SaveTheme(NxFr::StringView Name = "");

		GUI::Window* GetWindow();
		template<typename T> T* GetElement(NxFr::StringView Name) { return static_cast<T*>(GetElement(Name, T::GetClassType())); }
		GUI::Element* GetElement(NxFr::StringView Name, NxFr::StringView Id);
		template<typename T> T* GetPanel() { return static_cast<T*>(GetPanel(T::GetClassType())); }
		GUI::Panel* GetPanel(NxFr::StringId Type);
		GUI::Panel* GetActivePanel();

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
		void AddMenuWindowLayouts(NxFr::StringView Name);

		void LoadLayoutImGui(NxFr::StringView Path) const;
		void LoadLayoutNexus(NxFr::StringView Path) const;
		void SaveLayoutImGui(NxFr::StringView Path) const;
		void SaveLayoutNexus(NxFr::StringView Path) const;
		void LoadThemeImGui(const YAML::Node& Node) const;
		void LoadThemeNexus(const YAML::Node& Node);
		void SaveThemeImGui(YAML::Emitter& Emitter) const;
		void SaveThemeNexus(YAML::Emitter& Emitter) const;

	private:
		GUI::Window Window;
		NxFr::Set<GUI::Element*> Elements;
	};
}
