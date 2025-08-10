#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/GUI.h"

namespace NxEn
{
	namespace GUI
	{
#pragma region Element

		NEXUS_OBJECT_IMPLEMENTATION(Element)

		Element::Element()
			: Manual(false), WillClose(false)
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
			if (WillClose)
			{
				return;
			}

			WillClose = true;
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
			: GuiFlags(0), Title(""), Target(nullptr)
		{
		}

		Panel::~Panel()
		{
		}

		void Panel::ShowWithTarget(bool State, void* Instance)
		{
			if (Instance == nullptr)
			{
				Instance = FetchDefaultTarget();
			}

			SetEnabled(State);
			SetTarget(Instance);
		}

		void Panel::SetTarget(void* Instance)
		{
			Target = Instance;
		}

		void* Panel::GetTarget()
		{
			return Target;
		}

		Panel& Panel::SetGuiFlag(ImGuiWindowFlags GuiFlags)
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

			GuiFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
			Title = GetObjectType().ToString();
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

		void* Panel::FetchDefaultTarget() const
		{
			return nullptr;
		}

#pragma endregion

#pragma region Menu

		static const int64 MenuPriorityOffsetBase		= 10000;
		static const int64 MenuPriorityOffsetDelta		= 1000;
		static const int64 MenuPriorityOffsetProject	= MenuPriorityOffsetBase;
		static const int64 MenuPriorityOffsetTools		= MenuPriorityOffsetProject	+ MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetObject		= MenuPriorityOffsetTools	+ MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetWindow		= MenuPriorityOffsetObject	+ MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetEdit		= MenuPriorityOffsetWindow	+ MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetFile		= MenuPriorityOffsetEdit	+ MenuPriorityOffsetDelta;

		static const char* MenuPathProject				= "Project/";
		static const char* MenuPathTools				= "Tools/";
		static const char* MenuPathObject				= "Object/";
		static const char* MenuPathWindow				= "Window/";
		static const char* MenuPathEdit					= "Edit/";
		static const char* MenuPathFile					= "File/";

		NEXUS_OBJECT_IMPLEMENTATION(Menu)

		Menu::Item Menu::Item::Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, NxFr::StringView Shortcut, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			Item It(Callback, Validate, Path, Shortcut, Priority, Menu::ItemMode::Callback, 0, nullptr);
			GUISystem::RegisterMenuItem(&It);
			return It;
		}

		Menu::Item::Item(const NxFr::Delegate<void()>& Callback, const NxFr::Delegate<bool()>& Validate, NxFr::StringView Path, NxFr::StringView Shortcut, int64 Priority, ItemMode Mode, uint64 Index, void* Data)
			: Callback(Callback), Validate(Validate), Path(Path.ToString()), Shortcut(Shortcut.ToString()), Priority(Priority), Mode(Mode), Index(Index), Data(Data)
		{
			NEXUS_ASSERT(Priority > -MenuPriorityOffsetBase, Default, "Priority cannot go lower than the global nexus priority offset (%d)", MenuPriorityOffsetBase);

			if		(Path.Start(MenuPathProject))	Priority -= MenuPriorityOffsetProject;
			else if (Path.Start(MenuPathTools))		Priority -= MenuPriorityOffsetTools;
			else if (Path.Start(MenuPathObject))	Priority -= MenuPriorityOffsetObject;
			else if (Path.Start(MenuPathWindow))	Priority -= MenuPriorityOffsetWindow;
			else if (Path.Start(MenuPathEdit))		Priority -= MenuPriorityOffsetEdit;
			else if (Path.Start(MenuPathFile))		Priority -= MenuPriorityOffsetFile;

		}

		bool Menu::Item::operator==(const Item& Other) const
		{
			return Path == Other.Path;
		}

		bool Menu::Item::operator<=(const Item& Other) const
		{
			return Priority != Other.Priority ? Priority <= Other.Priority : Path <= Other.Path;
		}

		Menu::Menu(bool Main)
			: Items(), Labels(), Main(Main)
		{
			SetManual(true);
		}

		Menu::~Menu()
		{
		}

		Menu& Menu::AddMenuItem(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, NxFr::StringView Shortcut, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			AppendItem(Item(Callback, Validate, Path, Shortcut, Priority, ItemMode::Callback, 0, nullptr));
			return *this;
		}

		Menu& Menu::AddMenuToggle(NxFr::StringView Path, void* Toggle, const NxFr::Delegate<void()>& Callback, NxFr::StringView Shortcut, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			AppendItem(Item(Callback, Validate, Path, Shortcut, Priority, ItemMode::Toggle, 0, Toggle));
			return *this;
		}

		Menu& Menu::AddMenuEnum(NxFr::StringView Path, void* Enum, const NxFr::Array<NxFr::StringView>& Labels, const NxFr::Delegate<void()>& Callback, NxFr::StringView Shortcut, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			for (uint64 Index = 0; Index < Labels.GetCount(); ++Index)
			{
				AppendItem(Item(Callback, Validate, Path + NxFr::Path::SeparatorDirectory + Labels[Index], Shortcut, Priority, ItemMode::Enum, Index, Enum));
			}

			return *this;
		}

		Menu& Menu::Remove(NxFr::StringView Path)
		{
			Item* Instance = nullptr;
			for (auto& Item : Items)
			{
				if (Item.Path == Path)
				{
					Instance = &Item;
				}
			}

			if (Instance)
			{
				RemoveItem(*Instance);
			}

			return *this;
		}

		void Menu::OnTick(float TimeStep)
		{
			if (Main)
			{
				if (ImGui::BeginMainMenuBar())
				{
					DrawMenu(TimeStep);

					ImGui::EndMainMenuBar();
				}
			}
			else
			{
				if (ImGui::BeginMenuBar())
				{
					DrawMenu(TimeStep);

					ImGui::EndMenuBar();
				}
			}
		}

		void Menu::AppendItem(const Item& It)
		{
			Items.Append(It);
			Items.Sort();

			NxFr::List<NxFr::StringView> Sections = NxFr::Path::Split(It.Path);
			for (auto& Section : Sections)
			{
				NxFr::GUID Id = NxFr::Hash<>::HashObject(Section);
				if (!Labels.ContainsKey(Id))
				{
					Labels.Append(Id, Section.ToString());
				}
			}
		}

		void Menu::RemoveItem(const Item& It)
		{
			auto Iterator = Items.Find(It);
			Items.Remove(Iterator.Id());
		}

		void Menu::DrawMenu(float TimeStep)
		{
			for (auto& Item : Items)
			{
				NxFr::List<NxFr::StringView> Sections = NxFr::Path::Split(Item.Path);
				DrawItem(Item, Sections, 0);
			}

			OnGui(TimeStep);
		}

		void Menu::DrawItem(const Item& It, const NxFr::List<NxFr::StringView>& Sections, uint64 Depth) const
		{
			NxFr::GUID Id = NxFr::Hash<>::HashObject(Sections[Depth]);
			const NxFr::String& Lbl = Labels[Id];

			if (Depth == Sections.GetCount() - 1)
			{
				bool Call = false;
				bool Enabled = !It.Validate.IsNull() ? It.Validate.Invoke() : true;

				switch (It.Mode)
				{
				case ItemMode::Callback: Call = ImGui::MenuItem(Lbl.C(), It.Shortcut.C(), false, Enabled); break;
				case ItemMode::Enum:
					{
						int64& Current = *reinterpret_cast<int64*>(It.Data);
						Call = ImGui::MenuItem(Lbl.C(), It.Shortcut.C(), Current == It.Index, Enabled);
						if (Call)
						{
							Current = It.Index;
						}
					}
					break;
				case ItemMode::Toggle:
					{
						if (!Enabled)
						{
							ImGui::BeginDisabled();
						}
						Call = ImGui::Checkbox(Lbl.C(), reinterpret_cast<bool*>(It.Data));
						if (!Enabled)
						{
							ImGui::EndDisabled();
						}
					}
					break;
				}

				if (Call && !It.Callback.IsNull() && Enabled)
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
			: GuiFlags(0), Title(""), Message(""), Callbacks()
		{
		}

		Popup::~Popup()
		{
		}

		Popup& Popup::SetGuiFlag(ImGuiWindowFlags GuiFlags)
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

		Popup& Popup::AddButton(NxFr::StringView Label)
		{
			AddButton(Label, nullptr);
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

			GuiFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
		}

		void Popup::OnTick(float TimeStep)
		{
			ImGui::OpenPopup(Title.C());
			if (ImGui::BeginPopupModal(Title.C(), nullptr, GuiFlags))
			{
				if (!Message.IsEmpty())
				{
					ImGui::Text(Message.C());
				}

				OnGui(TimeStep);

				for (auto& Button : Callbacks)
				{
					if (ImGui::Button(Button.Label.C()))
					{
						if (!Button.Callback.IsNull())
						{
							Button.Callback.Invoke();
						}

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
			: GuiFlags(0), Title(""), Message(""), Callback(), Progress(0.0f)
		{
		}

		ProgressBar::~ProgressBar()
		{
		}

		ProgressBar& ProgressBar::SetGuiFlag(ImGuiWindowFlags GuiFlags)
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

		ProgressBar& ProgressBar::SetCallback(const NxFr::Delegate<void()>& Callback)
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

			GuiFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
		}

		void ProgressBar::OnTick(float TimeStep)
		{
			if (ImGui::Begin(Title.C(), nullptr, GuiFlags))
			{
				if (!Message.IsEmpty())
				{
					ImGui::Text(Message.C());
				}

				OnGui(TimeStep);

				float Percentag = ComputePercentage(TimeStep);
				ImGui::ProgressBar(Percentag);
				ImGui::End();
			}

			if (Progress >= 1.0f)
			{
				if (!Callback.IsNull())
				{
					Callback.Invoke();
				}
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
