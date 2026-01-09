#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"
#include "NexusEngine/Misc/Utils/Filter.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporterPopup.h"

#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionCreate.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDuplicate.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionRename.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionMove.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionDelete.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionLoad.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReload.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionImport.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReimport.h"
#include "NexusEditor/Systems/Assets/Browser/Actions/AssetsBrowserActionReferences.h"

namespace NxEd
{
	static AssetsBrowserPanel* Panel = NxEn::GUI::Panel::Create<AssetsBrowserPanel>();

	const static NxEn::GUI::Menu::Item MenuItemSettings = NxEn::GUI::Menu::Item::Create("Object/Assets/Browser", NxFr::Delegate<void()>([]()
	{
		NxEn::Application::GetSystem<NxEn::CommandsSystem>()->Execute("GUI.Panel AssetsBrowserPanel");
	}));

	const static NxEn::Command CmdAssetPing = NxEn::Command::Create("Assets.Ping"_Sid, "Ping path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Select(Path);
	}));
	const static NxEn::Command CmdAssetCreateDirectory = NxEn::Command::Create("Assets.Create.Directory"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Create(Path, 0);
	}));
	const static NxEn::Command CmdAssetCreateFile = NxEn::Command::Create("Assets.Create.File"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Type)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Create(Path, NxFr::StringId(Type));
	}));
	const static NxEn::Command CmdAssetMove = NxEn::Command::Create("Assets.Move"_Sid, "Move path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Move(Path, Target);
	}));
	const static NxEn::Command CmdAssetDuplicate = NxEn::Command::Create("Assets.Duplicate"_Sid, "Duplicate path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Duplicate(Path, Target);
	}));
	const static NxEn::Command CmdAssetDelete = NxEn::Command::Create("Assets.Delete"_Sid, "Delete path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->Delete(Path);
	}));

	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserPanel)

	AssetsBrowserPanel::AssetsBrowserPanel()
		: TreePanel(), Assets(nullptr)
	{
	}

	AssetsBrowserPanel::~AssetsBrowserPanel()
	{
	}

	void AssetsBrowserPanel::Select(NxFr::StringView Path, bool Additive, bool List)
	{
		TreePanel::Select(PathToId(ValidatePath(Path)), Additive, List);
	}

	void AssetsBrowserPanel::Create(NxFr::StringView Path, NxFr::StringId Type)
	{
		NEXUS_ASSERT(Path != RootPath, Default, "Can't create the Assets/ folder");

		NxFr::String AssetPath = PathToAsset(EnsureUniquePath(ValidatePath(Path)));

		AssetsBrowserItem* Item = AppendItem(AssetPath);
		Item->Create(Path, Type);
		AttachItem(Item, GetParent(AssetPath));
		SortItem(Item);
	}

	void AssetsBrowserPanel::Move(NxFr::StringView Path, NxFr::StringView Target)
	{
		NEXUS_ASSERT(Path != RootPath, Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = PathToId(ValidatePath(Path));

		AssetsBrowserItem* Item = GetItem(Id);
		DetachItem(Item);
		SortItem(Item);
		Item->Move(EnsureUniquePath(ValidatePath(Target)));
		AttachItem(Item, GetParent(Item->GetPath()));
		SortItem(Item);
	}

	void AssetsBrowserPanel::Duplicate(NxFr::StringView Path, NxFr::StringView Target)
	{
		NEXUS_ASSERT(Path != RootPath, Default, "Can't duplicate the Assets/ folder");

		NxFr::GUID Id = PathToId(ValidatePath(Path));

		AssetsBrowserItem* Item = GetItem(Id);
		Item = DuplicateItem(Item, nullptr);
		Item->Duplicate(EnsureUniquePath(ValidatePath(Target)));
		AttachItem(Item, GetParent(Item->GetPath()));
		SortItem(Item);
	}

	void AssetsBrowserPanel::Delete(NxFr::StringView Path)
	{
		NEXUS_ASSERT(Path != RootPath, Default, "Can't delete the Assets/ folder");

		NxFr::GUID Id = PathToId(ValidatePath(Path));

		AssetsBrowserItem* Item = GetItem(Id);
		Item->Delete();
		DetachItem(Item);
		RemoveItem(Item);
		SortItem(Item);
	}

	bool AssetsBrowserPanel::Exist(NxFr::StringView Path)
	{
		return NxFr::Path::Exist(PathToDisk(ValidatePath(Path)));
	}

	NxFr::StringView AssetsBrowserPanel::ValidatePath(NxFr::StringView Path)
	{
		if (Path == RootPath)
		{
			return RootPath;
		}

		if (NxFr::StringUtility::Start(Path, "/"))
		{
			Path = NxFr::StringUtility::TrimLeading(Path, '/');
		}

		NEXUS_ASSERT(NxFr::Path::IsDirectory(Path) || (NxFr::Path::IsFile(Path) && NxFr::Path::HasExtension(Path, "")),
			Default, "Path %s needs to have an extension or be a directory", Path.C());

		return Path;
	}

	NxFr::String AssetsBrowserPanel::EnsureUniquePath(NxFr::String Path)
	{
		bool Directory = NxFr::Path::IsDirectory(Path);
		NxFr::String Name = Directory ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path);
		NxFr::String Extension = NxFr::Path::GetExtension(Path);

		uint64 Count = 1;
		NxFr::String Target;
		while (Exist(Path))
		{
			Target = Name + " " + NxFr::StringUtility::ToString(Count);
			if (Directory)
			{
				NxFr::Path::ChangeDirectoryName(Path, Target);
			}
			else
			{
				Target += "." + Extension;
				NxFr::Path::ChangeFileName(Path, Target);
			}

			Count++;
		}

		return Path;
	}

	void AssetsBrowserPanel::OnInitialize()
	{
		TreePanel::OnInitialize();
		SetTitle("Assets Browser");

		AppendAction<AssetsBrowserActionCreate>();
		AppendAction<AssetsBrowserActionDuplicate>();
		AppendAction<AssetsBrowserActionRename>();
		AppendAction<AssetsBrowserActionMove>();
		AppendAction<AssetsBrowserActionDelete>();
		AppendAction<AssetsBrowserActionLoad>();
		AppendAction<AssetsBrowserActionReload>();
		AppendAction<AssetsBrowserActionImport>();
		AppendAction<AssetsBrowserActionReimport>();
		AppendAction<AssetsBrowserActionReferences>();
	}

	void AssetsBrowserPanel::OnEnable()
	{
		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		TreePanel::OnEnable();
	}

	AssetsBrowserItem* AssetsBrowserPanel::GetParent(NxFr::StringView Path)
	{
		NxFr::StringView Directory = NxFr::Path::GetParent(Path);
		if (Directory.IsEmpty())
		{
			Directory = RootPath;
		}

		NxFr::GUID DirectoryId = PathToId(Directory);
		return GetItem(DirectoryId);
	}

	AssetsBrowserItem* AssetsBrowserPanel::GetItem(NxFr::GUID Id)
	{
		NxEn::TreeItem* Item = TreePanel::GetItem(Id);
		return static_cast<AssetsBrowserItem*>(Item);
	}

	AssetsBrowserItem* AssetsBrowserPanel::FetchItems()
	{
		AssetsBrowserItem* Item = FetchItems(NxFr::Paths::Assets, nullptr);
		Item->ImGuiText = RootImGui;
		Item->Open(true);

		PurgeDuplicates(Item);
		return Item;
	}

	AssetsBrowserItem* AssetsBrowserPanel::FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent)
	{
		NxFr::String ItemPath = DiskToPath(Path);
		AssetsBrowserItem* Item = AppendItem(ItemPath);
		UpdateItem(Item, ItemPath, true, false);

		if (Parent)
		{
			Item->SetParent(Parent);
			if (Parent->GetChild() == nullptr)
			{
				Parent->SetChild(Item);
			}
		}

		if (Item->GetObjectType() == AssetsBrowserItemDirectory::GetClassType())
		{
			AssetsBrowserItem* Previous = nullptr;

			NxFr::Directory Directory(Path);
			for (auto& Path : Directory)
			{
				AssetsBrowserItem* Child = FetchItems(Path, Item);
				if (Previous)
				{
					Child->SetPrevious(Previous);
					Previous->SetNext(Child);
				}

				Previous = Child;
			}
		}

		return Item;
	}

	AssetsBrowserItem* AssetsBrowserPanel::PurgeDuplicates(AssetsBrowserItem* Item)
	{
		if (Item == nullptr)
		{
			return nullptr;
		}

		PurgeDuplicates(Item->GetChild());

		if (Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
		{
			if (Item->GetNext() && Item->GetPrettyName() == Item->GetNext()->GetPrettyName())
			{
				RemoveItem(Item->GetNext());
			}
			else if (Item->GetPrevious() && Item->GetPrettyName() == Item->GetPrevious()->GetPrettyName())
			{
				RemoveItem(Item->GetPrevious());
			}
		}

		while (Item)
		{
			Item = PurgeDuplicates(Item->GetNext());
		}

		return Item ? Item->GetNext() : nullptr;
	}

	AssetsBrowserItem* AssetsBrowserPanel::AppendItem(NxFr::StringView Path)
	{
		AssetsBrowserItem* Item = nullptr;
		if (NxFr::Path::IsDirectory(Path) || Path == RootPath)
		{
			Item = new AssetsBrowserItemDirectory();
		}
		else
		{
			if (NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::AssetExtension))
			{
				Item = new AssetsBrowserItemAsset();
			}
			else
			{
				Item = new AssetsBrowserItemFile();
			}
		}

		TreePanel::AppendItem(Item);
		return Item;
	}

	AssetsBrowserItem* AssetsBrowserPanel::DuplicateItem(NxEn::TreeItem* Item, NxEn::TreeItem* Parent)
	{
		if (!Item)
		{
			return nullptr;
		}

		AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
		AssetsBrowserItem* Copy = AppendItem(Instance->GetPath());
		UpdateItem(Copy, Instance->GetPath(), false, false);

		TreePanel::DuplicateItem(Copy, Item, Parent);
		return Copy;
	}

	void AssetsBrowserPanel::UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool AddId, bool RemoveId)
	{
		if (RemoveId)
		{
			if (Items.ContainsKey(Item->GetId()))
			{
				Items.Remove(Item->GetId());
			}
		}

		Item->Id = PathToId(Path);
		Item->Path = Path;
		Item->Type = Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType() ?
			Item->GetObjectType() : Assets->GetMetadata(Item->GetId()).GetType();
		Item->ImGuiText = Item->GetPrefix() + " " + Item->GetPrettyName() + "##" + NxFr::StringUtility::ToString(Item->Id);

		if (AddId)
		{
			Items.Append(Item->GetId(), Item);
		}
	}

	NxFr::GUID AssetsBrowserPanel::PathToId(NxFr::StringView Path)
	{
		if (Path == RootPath)
		{
			return NxFr::Hash<>::HashObject(RootImGui);
		}

		return NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::AssetExtension) ?
			Assets->PathToId(NxFr::Path::GetPathWithoutExtension(Path)) :
			NxFr::Hash<>::HashObject(Path);
	}

	NxFr::String AssetsBrowserPanel::DiskToPath(NxFr::StringView Path)
	{
		if (Path == NxFr::Paths::Assets)
		{
			return RootPath;
		}

		return NxFr::Path::ConvertAbsoluteToRelative(Path, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowserPanel::PathToDisk(NxFr::StringView Path)
	{
		if (Path == RootPath)
		{
			return NxFr::Paths::Assets.Data;
		}

		return NxFr::Path::ConvertRelativeToAbsolute(Path, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowserPanel::PathToAsset(NxFr::StringView Path)
	{
		return NxFr::Path::ChangeExtension(Path, NxEn::AssetMetadata::AssetExtension);
	}
}
