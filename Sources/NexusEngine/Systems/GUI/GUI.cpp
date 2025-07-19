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
			SetTitle(GetObjectType().C());
			SetPanelFlag(ImGuiWindowFlags_NoCollapse);
		}

		void Panel::OnTick(float TimeStep)
		{
			bool IsOpen = true;

			ImGui::Begin(GetTitle().C(), &IsOpen, PanelFlags);
			OnGui(TimeStep);
			ImGui::End();

			if (!IsOpen)
			{
				SetEnabled(false);
			}
		}

#pragma endregion

#pragma region Popup

		NEXUS_OBJECT_IMPLEMENTATION(Popup)

		Popup::Popup()
			: PanelFlags(0), Title(""), Message(""), Callbacks()
		{
		}

		Popup::~Popup()
		{
		}

		Popup& Popup::SetTitle(NxFr::StringView Title)
		{
			this->Title = Title.ToString();
			return *this;
		}

		Popup& Popup::SetMessage(NxFr::StringView Message)
		{
			this->Message = Message.ToString();
			return *this;
		}

		Popup& Popup::AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback)
		{
			Callbacks.AppendConstruct(NxFr::Tuple<NxFr::String, NxFr::Delegate<void()>>(Label.ToString(), Callback));
			return *this;
		}

		void Popup::OnInitialize()
		{
			PanelFlags |= ImGuiWindowFlags_NoCollapse;
			PanelFlags |= ImGuiWindowFlags_NoDocking;
		}

		void Popup::OnTick(float TimeStep)
		{
			ImGui::Begin(Title.C(), nullptr, PanelFlags);

			ImGui::Text(Message.C());
			OnGui(TimeStep);
			for (auto& Button : Callbacks)
			{
				if (ImGui::Button(Button.GetFirst().C(), { 100, 50 } ))
				{
					Button.GetSecond().Invoke();
					Close();
				}
			}

			ImGui::End();
		}

		void Popup::Close()
		{
			Application::GetInstance()->GetTicker().AppendTickOnceCallback([=]() { Object::Destroy(this); }, Ticker::TickBucket::Cleanup);
		}

#pragma endregion
	}
}
