#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/GUI/Style.h"
#include "NexusEngine/Systems/GUI/Utils.h"
#include "NexusEngine/Systems/GUI/Draw.h"
#include "NexusEngine/Systems/GUI/Element.h"
#include "NexusEngine/Systems/GUI/Misc/Styles.h"
#include "NexusEngine/Systems/GUI/Misc/Draw.h"
#include "NexusEngine/Systems/GUI/Misc/Serialization.h"

namespace NxEn
{
	class NX_ENGINE_API GUISystem : public System
	{
		friend class GUI::Element;

	public:
		NX_OBJECT(GUISystem)

		static NxFr::Registry<GUI::Panel*>& GetPanels();
		static NxFr::Registry<GUI::Menu::Item>& GetMenuItems();

		GUISystem();
		~GUISystem();

		void LoadLayout(NxFr::StringView Name = "");
		void SaveLayout(NxFr::StringView Name = "");
		void LoadTheme(NxFr::StringView Name = "");
		void SaveTheme(NxFr::StringView Name = "");

		GUI::Window* GetWindow();
		GUI::Element* GetElement(NxFr::GUID Id);
		template<typename T> T* GetPanel() { return static_cast<T*>(GetPanel(T::GetClassType())); }
		GUI::Panel* GetPanel(NxFr::StringId Type);
		GUI::Panel* GetActivePanel();

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void DrawElement(GUI::Element* Element, bool State);
		void DestroyElement(GUI::Element* Element);

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
		NxFr::Set<GUI::Element*> Drawing;
		NxFr::Set<GUI::Element*> Destroyed;
	};
}
