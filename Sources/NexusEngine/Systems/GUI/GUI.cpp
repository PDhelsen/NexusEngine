#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	namespace GUI
	{
#pragma region Element

		NEXUS_OBJECT_IMPLEMENTATION(Element)

		void Element::OnTick(float TimeStep)
		{
			OnGui(TimeStep);
		}

		void Element::OnEnable()
		{
			Application::GetSystem<GUISystem>()->GetOnGui() += NxFr::Delegate<void(float)>(this, &Element::OnTick);
		}

		void Element::OnDisable()
		{
			Application::GetSystem<GUISystem>()->GetOnGui() -= NxFr::Delegate<void(float)>(this, &Element::OnTick);
		}

#pragma endregion

#pragma region Panel

		NEXUS_OBJECT_IMPLEMENTATION(Panel)

		Panel::Panel()
			: Title(""), PanelFlags(0)
		{
		}

		Panel::~Panel()
		{
		}

		Panel& Panel::SetTitle(NxFr::StringView Title)
		{
			this->Title = Title.ToString();
			return *this;
		}

		Panel& Panel::SetPanelFlag(ImGuiWindowFlags_ PanelFlags)
		{
			this->PanelFlags |= PanelFlags;
			return *this;
		}

		void Panel::OnInitialize()
		{
			Title = GetObjectType().C();
		}

		void Panel::OnTick(float TimeStep)
		{
			ImGui::Begin(GetTitle().C(), nullptr, PanelFlags);
			Element::OnTick(TimeStep);
			ImGui::End();
		}
	}

#pragma endregion
}
