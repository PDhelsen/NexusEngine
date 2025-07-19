#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	namespace GUI
	{
#pragma region Element

		NEXUS_OBJECT_IMPLEMENTATION(Element)

		Element::Element(bool Manual)
			: Manual(Manual)
		{
		}

		Element::~Element()
		{
		}

		void Element::Show()
		{
			SetEnabled(true);
		}

		void Element::Hide()
		{
			SetEnabled(false);
		}

		void Element::Close()
		{
			Hide();
			Application::GetInstance()->GetTicker().AppendTickOnceCallback([=]() { Object::Destroy(this); }, Ticker::TickBucket::Cleanup);
		}

		void Element::OnInitialize()
		{
			SetTickable(true);
		}

		void Element::OnTick(float TimeStep)
		{
			OnGui(TimeStep);
		}

		void Element::OnEnable()
		{
			if (Manual)
			{
				return;
			}

			Application::GetSystem<GUISystem>()->GetOnGui() += NxFr::Delegate<void(float)>(this, &Element::Tick);
		}

		void Element::OnDisable()
		{
			if (Manual)
			{
				return;
			}

			Application::GetSystem<GUISystem>()->GetOnGui() -= NxFr::Delegate<void(float)>(this, &Element::Tick);
		}

#pragma endregion

#pragma region Panel

		NEXUS_OBJECT_IMPLEMENTATION(Panel)

		Panel::Panel()
			: Element(false), Title(""), PanelFlags(0)
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
			Element::OnInitialize();

			SetTitle(GetObjectType().C());
			SetPanelFlag(ImGuiWindowFlags_NoCollapse);
		}

		void Panel::OnTick(float TimeStep)
		{
			bool IsOpen = true;

			if (ImGui::Begin(GetTitle().C(), &IsOpen, PanelFlags))
			{
				OnGui(TimeStep);
				ImGui::End();
			}

			if (!IsOpen)
			{
				Hide();
			}
		}

		void Panel::OnGui(float TimeStep)
		{
		}

#pragma endregion

#pragma region Popup

		NEXUS_OBJECT_IMPLEMENTATION(Popup)

		Popup::Popup()
			: Element(false), PanelFlags(0), Title(""), Message(""), Callbacks()
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
			Element::OnInitialize();

			PanelFlags |= ImGuiWindowFlags_NoCollapse;
			PanelFlags |= ImGuiWindowFlags_NoDocking;
		}

		void Popup::OnTick(float TimeStep)
		{
			ImGui::OpenPopup(Title.C());
			if (ImGui::BeginPopupModal(Title.C(), nullptr, PanelFlags))
			{
				OnGui(TimeStep);

				for (auto& Button : Callbacks)
				{
					if (ImGui::Button(Button.GetFirst().C(), { 100, 50 }))
					{
						Button.GetSecond().Invoke();
						Close();

						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}

		void Popup::OnGui(float TimeStep)
		{
			ImGui::Text(Message.C());
		}

#pragma endregion

#pragma region Progress

		NEXUS_OBJECT_IMPLEMENTATION(ProgressBar)

		ProgressBar::ProgressBar()
			: Element(false), PanelFlags(0), Title(""), Message(""), Callback(), Progress(0.0f)
		{
		}

		ProgressBar::~ProgressBar()
		{
		}

		ProgressBar& ProgressBar::SetTitle(NxFr::StringView Title)
		{
			this->Title = Title.ToString();
			return *this;
		}

		ProgressBar& ProgressBar::SetMessage(NxFr::StringView Message)
		{
			this->Message = Message.ToString();
			return *this;
		}

		ProgressBar& ProgressBar::SetCallback(NxFr::Delegate<void()> Callback)
		{
			this->Callback = Callback;
			return *this;
		}

		ProgressBar& ProgressBar::SetProgress(float Progress)
		{
			this->Progress = Progress;
			return *this;
		}

		void ProgressBar::OnInitialize()
		{
			Element::OnInitialize();

			PanelFlags |= ImGuiWindowFlags_NoCollapse;
			PanelFlags |= ImGuiWindowFlags_NoDocking;
		}

		void ProgressBar::OnTick(float TimeStep)
		{
			if (ImGui::Begin(GetTitle().C(), nullptr, PanelFlags))
			{
				OnGui(TimeStep);

				float Percentag = ComputePercentage(TimeStep);
				ImGui::ProgressBar(Percentag);
				ImGui::End();
			}

			if (Progress >= 1.0f)
			{
				Callback.Invoke();
				Hide();
			}
		}

		void ProgressBar::OnGui(float TimeStep)
		{
			ImGui::Text(Message.C());
		}

		float ProgressBar::ComputePercentage(float TimeStep)
		{
			if (Progress >= 0.0f)
			{
				return Progress;
			}

			Progress -= TimeStep;
			Progress = NxFr::Math::FMod(Progress, 1.0f);

			return NxFr::Math::Abs(Progress);
		}

#pragma endregion
	}
}
