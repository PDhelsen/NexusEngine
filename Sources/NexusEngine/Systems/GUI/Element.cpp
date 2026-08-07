#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Element.h"

namespace NxEn
{
	namespace GUI
	{
#pragma region Element

		Element::Element()
			: Id(0), Name(), NamedId(),
			GuiFlags(ElementFlags::None), ImGuiFlags(0)
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
			if (GetGuiFlag(ElementFlags::HideInsteadOfClose))
			{
				Hide();
				return;
			}

			if (GetGuiFlag(ElementFlags::WillClose))
			{
				return;
			}

			Application::GetSystem<GUISystem>()->DestroyElement(this);
			SetGuiFlag(ElementFlags::WillClose, true);
		}

		void Element::SetNameId(NxFr::StringView Name, NxFr::GUID Id)
		{
			if (Id == 0)
			{
				Id = GetObjectType();
			}

			NamedId = Utils::NexusToImGuiId(Name, NxFr::StringUtility::ToString(Id));
			this->Name = Utils::ImGuiToNexusName(NamedId);
			this->Id = Id;
		}

		void Element::OnInitialize()
		{
			SetGuiFlag(ElementFlags::AutoDraw, true);
			SetNameId(GetObjectType());
		}

		void Element::OnEnable()
		{
			if (!IsAutoDraw())
			{
				return;
			}

			Application::GetSystem<GUISystem>()->DrawElement(this, true);
		}

		void Element::OnDisable()
		{
			Application::GetSystem<GUISystem>()->DrawElement(this, false);
		}

		bool Element::GetGuiFlag(ElementFlags Flag) const
		{
			return NxFr::Enum::CheckFlag(GuiFlags, Flag);
		}

		void Element::SetGuiFlag(ElementFlags Flag, bool Value)
		{
			GuiFlags = NxFr::Enum::SetFlag(GuiFlags, Flag, Value);
		}

		bool Element::GetImGuiFlag(uint64 Flag) const
		{
			return NxFr::Integer::CheckFlag(ImGuiFlags, Flag);
		}

		void Element::SetImGuiFlag(uint64 Flag, bool Value)
		{
			ImGuiFlags = NxFr::Integer::SetFlag(ImGuiFlags, Flag, Value);
		}

		uint64 Element::GetImGuiFlags() const
		{
			return ImGuiFlags;
		}

		void Element::SetImGuiFlags(uint64 Flags)
		{
			ImGuiFlags = Flags;
		}

#pragma endregion

#pragma region Panel

		Panel::Panel()
		{
		}

		Panel::~Panel()
		{
		}

		void Panel::Draw()
		{
			bool IsOpen = true;

			if (!Dock.IsEmpty())
			{
				ImGui::SetNextWindowDockID(ImGui::GetID(Dock.C()), ImGuiCond_FirstUseEver);
			}

			if (ImGui::Begin(GetNamedId().C(), &IsOpen, GetImGuiFlags()))
			{
				OnDraw();
			}
			ImGui::End();

			if (!IsOpen)
			{
				Close();
			}
		}

		Panel& Panel::SetDock(NxFr::StringView Id)
		{
			this->Dock = Id;
			return *this;
		}

		void Panel::OnInitialize()
		{
			Element::OnInitialize();

			SetGuiFlag(ElementFlags::HideInsteadOfClose, true);
			SetImGuiFlag(ImGuiWindowFlags_NoCollapse, true);
		}

#pragma endregion

#pragma region Menu

		const Menu::Item* Menu::Item::Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			return GUISystem::GetMenuItems().Register(Path, Item(Callback, Validate, Path, Priority, ItemMode::Callback, 0, nullptr));
		}

		Menu::Item::Item(const NxFr::Delegate<void()>& Callback, const NxFr::Delegate<bool()>& Validate, NxFr::StringView Path, int64 Priority, ItemMode Mode, uint64 Index, void* Data)
			: Callback(Callback), Validate(Validate), Path(Path), Priority(Priority), Mode(Mode), Index(Index), Data(Data)
		{
		}

		bool Menu::Item::operator==(const Item& Other) const
		{
			return Path == Other.Path;
		}

		bool Menu::Item::operator<=(const Item& Other) const
		{
			return Priority != Other.Priority ? Priority <= Other.Priority : Path <= Other.Path;
		}

		static const int64 MenuPriorityOffsetBase		= 10000;
		static const int64 MenuPriorityOffsetDelta		= 1000;
		static const int64 MenuPriorityOffsetProject	= MenuPriorityOffsetBase;
		static const int64 MenuPriorityOffsetTools		= MenuPriorityOffsetProject + MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetObject		= MenuPriorityOffsetTools + MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetWindow		= MenuPriorityOffsetObject + MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetEdit		= MenuPriorityOffsetWindow + MenuPriorityOffsetDelta;
		static const int64 MenuPriorityOffsetFile		= MenuPriorityOffsetEdit + MenuPriorityOffsetDelta;

		static const char* MenuPathProject	= "Project/";
		static const char* MenuPathTools	= "Tools/";
		static const char* MenuPathObject	= "Object/";
		static const char* MenuPathWindow	= "Window/";
		static const char* MenuPathEdit		= "Edit/";
		static const char* MenuPathFile		= "File/";

		int64 Menu::ComputePriority(NxFr::StringView Path, int64 Priority)
		{
			NX_ASSERT(Priority > -MenuPriorityOffsetBase, Default, "Priority cannot go lower than the global nexus priority offset (%lld)", MenuPriorityOffsetBase);

			if		(NxFr::StringUtility::Start(Path, MenuPathProject))	Priority -= MenuPriorityOffsetProject;
			else if (NxFr::StringUtility::Start(Path, MenuPathTools))	Priority -= MenuPriorityOffsetTools;
			else if (NxFr::StringUtility::Start(Path, MenuPathObject))	Priority -= MenuPriorityOffsetObject;
			else if (NxFr::StringUtility::Start(Path, MenuPathWindow))	Priority -= MenuPriorityOffsetWindow;
			else if (NxFr::StringUtility::Start(Path, MenuPathEdit))	Priority -= MenuPriorityOffsetEdit;
			else if (NxFr::StringUtility::Start(Path, MenuPathFile))	Priority -= MenuPriorityOffsetFile;

			return Priority;
		}

		Menu::Menu(bool Main)
			: Items(), Labels(), Main(Main)
		{
		}

		Menu::~Menu()
		{
		}

		void Menu::Draw()
		{
			if (Main)
			{
				if (ImGui::BeginMainMenuBar())
				{
					DrawMenu();
				}
				ImGui::EndMainMenuBar();
			}
			else
			{
				if (ImGui::BeginMenuBar())
				{
					DrawMenu();
				}
				ImGui::EndMenuBar();
			}
		}

		Menu& Menu::AddMenuItem(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			AppendItem(Item(Callback, Validate, Path, Priority, ItemMode::Callback, 0, nullptr));
			return *this;
		}

		Menu& Menu::AddMenuToggle(NxFr::StringView Path, void* Toggle, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			AppendItem(Item(Callback, Validate, Path, Priority, ItemMode::Toggle, 0, Toggle));
			return *this;
		}

		Menu& Menu::AddMenuEnum(NxFr::StringView Path, void* Enum, const NxFr::Array<NxFr::StringView>& Labels, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			for (uint64 Index = 0; Index < Labels.GetCount(); ++Index)
			{
				AppendItem(Item(Callback, Validate, NxFr::Path::Combine(Path, Labels[Index]), Priority, ItemMode::Enum, Index, Enum));
			}

			return *this;
		}

		Menu& Menu::Remove(NxFr::StringView Path)
		{
			NxFr::List<uint64> ToRemove;
			for (uint64 Index = 0; Index < Items.GetCount(); ++Index)
			{
				Item& Item = Items[Index];
				if (NxFr::StringUtility::Contains(Item.Path, Path))
				{
					ToRemove.Append(Index);
				}
			}

			for (uint64 Index = ToRemove.GetCount(); Index > 0; --Index)
			{
				RemoveItem(Items[ToRemove[Index - 1]]);
			}

			return *this;
		}

		Menu& Menu::Clear()
		{
			Items.Clear();
			return *this;
		}

		void Menu::OnInitialize()
		{
			Element::OnInitialize();

			SetGuiFlag(ElementFlags::AutoDraw, false);
			SetNameId(GetObjectType(), NxFr::Integer::GenerateGuid());
		}

		void Menu::OnShutdown()
		{
			Items.Clear();
			Labels.Clear();
		}

		void Menu::AppendItem(const Item& It)
		{
			Items.Append(It);
			NxFr::ContainerUtility::Sort<NxEn::GUI::Menu::Item>(Items);

			NxFr::List<NxFr::StringView> Sections = NxFr::Path::Split(It.Path);
			for (auto& Section : Sections)
			{
				NxFr::GUID Id = NxFr::Hash<>::HashObject(Section);
				if (!Labels.TryGet(Id))
				{
					Labels.Append(Id, Section);
				}
			}
		}

		void Menu::RemoveItem(const Item& It)
		{
			auto Iterator = NxFr::ContainerUtility::Find(Items, It);
			if (Iterator != Items.End())
			{
				Items.Remove(Iterator.Id());
			}
		}

		void Menu::DrawMenu()
		{
			for (uint64 Index = 0; Index < Items.GetCount(); ++Index)
			{
				Item& Item = Items[Index];
				NxFr::List<NxFr::StringView> Sections = NxFr::Path::Split(Item.Path);
				DrawItem(Item, Sections, 0);
			}

			OnDraw();
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
				case ItemMode::Callback: Call = ImGui::MenuItem(Lbl.C(), "", false, Enabled); break;
				case ItemMode::Enum:
					{
						int64& Current = *reinterpret_cast<int64*>(It.Data);
						Call = ImGui::MenuItem(Lbl.C(), "", Current == It.Index, Enabled);
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

		Popup::Popup()
			: Message(""), Callbacks()
		{
		}

		Popup::~Popup()
		{
		}

		void Popup::Draw()
		{
			ImGui::OpenPopup(GetNamedId().C());
			if (ImGui::BeginPopupModal(GetNamedId().C(), nullptr, GetImGuiFlags()))
			{
				if (!Message.IsEmpty())
				{
					ImGui::SetCursorPosX(GUI::Utils::Center(Message).x);
					ImGui::Text(Message.C());
				}

				OnDraw();

				ImGui::SetCursorPosX(GUI::Utils::Center(NxFr::Vector2f(GUI::Styles::WidthButton() * Callbacks.GetCount(), 0), Callbacks.GetCount() - 1).x);
				for (uint64 Index = 0; Index < Callbacks.GetCount(); ++Index)
				{
					Item& Button = Callbacks[Index];
					if (ImGui::Button(Button.Label.C(), { GUI::Styles::WidthButton(), 0.0f }))
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
			}
			ImGui::EndPopup();
		}

		Popup& Popup::SetMessage(NxFr::StringView Message)
		{
			this->Message = Message;
			return *this;
		}

		Popup& Popup::AddButton(NxFr::StringView Label)
		{
			AddButton(Label, nullptr);
			return *this;
		}

		Popup& Popup::AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback)
		{
			Callbacks.AppendConstruct(Callback, Label);
			return *this;
		}

		Popup& Popup::Clear()
		{
			Callbacks.Clear();
			return *this;
		}

		void Popup::OnInitialize()
		{
			Element::OnInitialize();

			SetGuiFlag(ElementFlags::HideInsteadOfClose, true);
			SetImGuiFlag(ImGuiWindowFlags_NoCollapse, true);
			SetImGuiFlag(ImGuiWindowFlags_NoDocking, true);
		}

#pragma endregion

#pragma region Progress

		ProgressBar::ProgressBar()
			: Message(""), Callback(), Progress(0.0f)
		{
		}

		ProgressBar::~ProgressBar()
		{
		}

		void ProgressBar::Draw()
		{
			if (ImGui::Begin(GetNamedId().C(), nullptr, GetImGuiFlags()))
			{
				if (!Message.IsEmpty())
				{
					ImGui::Text(Message.C());
				}

				OnDraw();

				float TimeStep = Application::GetInstance()->GetTime().GetDeltaTime();
				float Percentag = ComputePercentage(TimeStep);
				ImGui::ProgressBar(Percentag);
			}
			ImGui::End();

			if (Progress >= 1.0f)
			{
				if (!Callback.IsNull())
				{
					Callback.Invoke();
				}
				Close();
			}
		}

		ProgressBar& ProgressBar::SetMessage(NxFr::StringView Message)
		{
			this->Message = Message;
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

			SetImGuiFlag(ImGuiWindowFlags_NoCollapse, true);
			SetImGuiFlag(ImGuiWindowFlags_NoDocking, true);
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

#pragma region Window

		Window::Window()
			: MainMenu(true)
		{
			SetAutoDraw(false);
		}

		Window::~Window()
		{
		}

		void Window::Draw()
		{
			MainMenu.Draw();

			ImGui::DockSpaceOverViewport(ImGui::GetID(GetNamedId().C()));

			OnDraw();
		}

		void Window::OnInitialize()
		{
			Element::OnInitialize();
			MainMenu.Initialize();

			SetGuiFlag(ElementFlags::AutoDraw, false);
			SetImGuiFlags(
				ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
				ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings
				);
		}

		void Window::OnShutdown()
		{
			MainMenu.Shutdown();
			Element::OnShutdown();
		}

#pragma endregion
	}
}
