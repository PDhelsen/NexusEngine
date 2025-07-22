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
			Application::GetSystem<GUISystem>()->RegisterElement(this);
		}

		void Element::OnDisable()
		{
			Application::GetSystem<GUISystem>()->UnregisterElement(this);
		}

#pragma endregion

#pragma region Panel

		NEXUS_OBJECT_IMPLEMENTATION(Panel)

		Panel::Panel()
			: Element(false), GuiFlags(0), Title("")
		{
		}

		Panel::~Panel()
		{
		}

		Panel& Panel::SetGuiFlag(ImGuiWindowFlags_ GuiFlags)
		{
			this->GuiFlags |= GuiFlags;
			return *this;
		}

		Panel& Panel::SetTitle(NxFr::StringView Title)
		{
			this->Title = Title.ToString();
			return *this;
		}

		void Panel::OnInitialize()
		{
			Element::OnInitialize();

			GuiFlags = GetDefaultFlags();
			Title = GetDefaultTile();
		}

		void Panel::OnTick(float TimeStep)
		{
			bool IsOpen = true;

			if (ImGui::Begin(Title.C(), &IsOpen, GuiFlags))
			{
				OnGui(TimeStep);
				ImGui::End();
			}

			if (!IsOpen)
			{
				Hide();
			}
		}

#pragma endregion

#pragma region Menu

		NEXUS_OBJECT_IMPLEMENTATION(Menu)

		bool Menu::Item::operator<=(const Item& Other) const
		{
			return Priority != Other.Priority ? Priority <= Other.Priority : Path <= Other.Path;
		}

		Menu::Menu()
			: Element(true), Items(), Labels()
		{
		}

		Menu::~Menu()
		{
		}

		Menu& Menu::AddMenuItem(NxFr::Delegate<void()> Callback, NxFr::StringView Path, NxFr::StringView Shortcut, int64 Priority)
		{
			Items.AppendConstruct(Callback, Path.ToString(), Shortcut.ToString(), Priority);
			Items.Sort();

			NxFr::List<NxFr::StringView> Sections = NxFr::Path::Split(Path);
			for (auto& Section : Sections)
			{
				NxFr::GUID Id = NxFr::Hash<>::HashObject(Section);
				Labels.Append(Id, Section.ToString());
			}

			return *this;
		}

		void Menu::OnTick(float TimeStep)
		{
			if (ImGui::BeginMenuBar())
			{
				for (auto& Item : Items)
				{
					NxFr::List<NxFr::StringView> Sections = NxFr::Path::Split(Item.Path);
					DrawItem(Item, Sections, 0);
				}

				OnGui(TimeStep);

				ImGui::EndMenuBar();
			}
		}

		void Menu::DrawItem(const Item& It, const NxFr::List<NxFr::StringView>& Sections, uint64 Depth) const
		{
			NxFr::GUID Id = NxFr::Hash<>::HashObject(Sections[Depth]);
			const NxFr::String& Lbl = Labels[Id];

			if (Depth == Sections.GetCount() - 1)
			{
				if (ImGui::MenuItem(Lbl.C(), It.Shortcut.C()))
				{
					It.Callback.Invoke();
				}

				return;
			}

			if (ImGui::BeginMenu(Lbl.C()))
			{
				DrawItem(It, Sections, ++Depth);
				ImGui::EndMenu();
			}
		}

#pragma endregion

#pragma region Popup

		NEXUS_OBJECT_IMPLEMENTATION(Popup)

		Popup::Popup()
			: Element(false), GuiFlags(0), Title(""), Message(""), Callbacks()
		{
		}

		Popup::~Popup()
		{
		}

		Popup& Popup::SetGuiFlag(ImGuiWindowFlags_ GuiFlags)
		{
			this->GuiFlags |= GuiFlags;
			return *this;
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
			Callbacks.AppendConstruct(Callback, Label.ToString());
			return *this;
		}

		void Popup::OnInitialize()
		{
			Element::OnInitialize();

			GuiFlags = GetDefaultFlags();
			Title = GetDefaultTile();
		}

		void Popup::OnTick(float TimeStep)
		{
			ImGui::OpenPopup(Title.C());
			if (ImGui::BeginPopupModal(Title.C(), nullptr, GuiFlags))
			{
				ImGui::Text(Message.C());

				OnGui(TimeStep);

				for (auto& Button : Callbacks)
				{
					if (ImGui::Button(Button.Label.C()))
					{
						Button.Callback.Invoke();
						Close();

						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();
				}

				ImGui::EndPopup();
			}
		}

#pragma endregion

#pragma region Progress

		NEXUS_OBJECT_IMPLEMENTATION(ProgressBar)

		ProgressBar::ProgressBar()
			: Element(false), GuiFlags(0), Title(""), Message(""), Callback(), Progress(0.0f)
		{
		}

		ProgressBar::~ProgressBar()
		{
		}

		ProgressBar& ProgressBar::SetGuiFlag(ImGuiWindowFlags_ GuiFlags)
		{
			this->GuiFlags = GuiFlags;
			return *this;
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

			GuiFlags = GetDefaultFlags();
			Title = GetDefaultTile();
		}

		void ProgressBar::OnTick(float TimeStep)
		{
			if (ImGui::Begin(Title.C(), nullptr, GuiFlags))
			{
				ImGui::Text(Message.C());

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
