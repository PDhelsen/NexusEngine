#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/GUI/Elements/Menu.h"

namespace NxEn
{
	namespace GUI
	{
		const Menu::Item* Menu::Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			return GUISystem::GetMenuItems().Register(Path, Menu::Item(Callback, Validate, Path, Priority, Menu::ItemMode::Callback, 0, nullptr));
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

		int64 Menu::MainMenuPriority(NxFr::StringView Path, int64 Priority)
		{
			static const int64 MenuPriorityOffsetBase = 10000;
			static const int64 MenuPriorityOffsetDelta = 1000;
			static const int64 MenuPriorityOffsetProject = MenuPriorityOffsetBase;
			static const int64 MenuPriorityOffsetTools = MenuPriorityOffsetProject + MenuPriorityOffsetDelta;
			static const int64 MenuPriorityOffsetObject = MenuPriorityOffsetTools + MenuPriorityOffsetDelta;
			static const int64 MenuPriorityOffsetWindow = MenuPriorityOffsetObject + MenuPriorityOffsetDelta;
			static const int64 MenuPriorityOffsetEdit = MenuPriorityOffsetWindow + MenuPriorityOffsetDelta;
			static const int64 MenuPriorityOffsetFile = MenuPriorityOffsetEdit + MenuPriorityOffsetDelta;

			static const char* MenuPathProject = "Project/";
			static const char* MenuPathTools = "Tools/";
			static const char* MenuPathObject = "Object/";
			static const char* MenuPathWindow = "Window/";
			static const char* MenuPathEdit = "Edit/";
			static const char* MenuPathFile = "File/";

			NX_ASSERT(Priority > -MenuPriorityOffsetBase, Default, "Priority cannot go lower than the global nexus priority offset (%lld)", MenuPriorityOffsetBase);

			if (NxFr::StringUtility::Start(Path, MenuPathProject))	Priority -= MenuPriorityOffsetProject;
			else if (NxFr::StringUtility::Start(Path, MenuPathTools))	Priority -= MenuPriorityOffsetTools;
			else if (NxFr::StringUtility::Start(Path, MenuPathObject))	Priority -= MenuPriorityOffsetObject;
			else if (NxFr::StringUtility::Start(Path, MenuPathWindow))	Priority -= MenuPriorityOffsetWindow;
			else if (NxFr::StringUtility::Start(Path, MenuPathEdit))	Priority -= MenuPriorityOffsetEdit;
			else if (NxFr::StringUtility::Start(Path, MenuPathFile))	Priority -= MenuPriorityOffsetFile;

			return Priority;
		}

		Menu::Menu()
			: Items(), Labels()
		{
		}

		Menu::~Menu()
		{
		}

		void Menu::Draw()
		{
			if (!IsEnabled())
			{
				return;
			}

			if (GetGuiFlag(ElementFlags::MainMenuBar))
			{
				if (ImGui::BeginMainMenuBar())
				{
					DrawMenu();
					OnDraw();
				}
				ImGui::EndMainMenuBar();
			}
			else
			{
				if (ImGui::BeginMenuBar())
				{
					DrawMenu();
					OnDraw();
				}
				ImGui::EndMenuBar();
			}
		}

		void Menu::AddMenuItem(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			AppendItem(Item(Callback, Validate, Path, Priority, ItemMode::Callback, 0, nullptr));
		}

		void Menu::AddMenuToggle(NxFr::StringView Path, void* Toggle, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			AppendItem(Item(Callback, Validate, Path, Priority, ItemMode::Toggle, 0, Toggle));
		}

		void Menu::AddMenuEnum(NxFr::StringView Path, void* Enum, const NxFr::Array<NxFr::StringView>& Labels, const NxFr::Delegate<void()>& Callback, int64 Priority, const NxFr::Delegate<bool()>& Validate)
		{
			for (uint64 Index = 0; Index < Labels.GetCount(); ++Index)
			{
				AppendItem(Item(Callback, Validate, NxFr::Path::Combine(Path, Labels[Index]), Priority, ItemMode::Enum, Index, Enum));
			}
		}

		void Menu::Remove(NxFr::StringView Path)
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
		}

		void Menu::Clear()
		{
			Items.Clear();
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

			Element::OnShutdown();
		}

		void Menu::AppendItem(const Item& It)
		{
			Items.Append(It);
			NxFr::ContainerUtility::Sort<NxEn::GUI::Menu::Item>(Items);

			NxFr::List<NxFr::StringView> Sections = NxFr::Path::Split(It.Path);
			for (auto& Section : Sections)
			{
				NxFr::GUID Id = NxFr::Hash<>::HashObject(Section);
				Labels.TryAppend(Id, Section);
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
	}
}
