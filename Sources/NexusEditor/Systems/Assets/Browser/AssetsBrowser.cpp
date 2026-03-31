#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusEditor/Systems/Editor/EditorSystem.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEd
{
	const static NxEn::Command CmdAssetBrowserCreateDirectory = NxEn::Command::Create("Assets.Browser.Create.Directory"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Item)
	{
		NxEn::Application::GetSystem<EditorSystem>()->GetAssetsBrowser().Create(Item, 0);
	}));
	const static NxEn::Command CmdAssetBrowserCreateFile = NxEn::Command::Create("Assets.Browser.Create.File"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Item, NxFr::StringView Type)
	{
		NxEn::Application::GetSystem<EditorSystem>()->GetAssetsBrowser().Create(Item, NxFr::StringId(Type));
	}));
	const static NxEn::Command CmdAssetBrowserMove = NxEn::Command::Create("Assets.Browser.Move"_Sid, "Move path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Item, NxFr::StringView Target)
	{
		NxEn::Application::GetSystem<EditorSystem>()->GetAssetsBrowser().Move(Item, Target);
	}));
	const static NxEn::Command CmdAssetBrowserDuplicate = NxEn::Command::Create("Assets.Browser.Duplicate"_Sid, "Duplicate path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Item, NxFr::StringView Target)
	{
		NxEn::Application::GetSystem<EditorSystem>()->GetAssetsBrowser().Duplicate(Item, Target);
	}));
	const static NxEn::Command CmdAssetBrowserDelete = NxEn::Command::Create("Assets.Browser.Delete"_Sid, "Delete path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Item)
	{
		NxEn::Application::GetSystem<EditorSystem>()->GetAssetsBrowser().Delete(Item);
	}));

	AssetsBrowser::AssetsBrowser()
		: Assets(NxEn::Application::GetSystem<NxEn::AssetsSystem>()), Items(), Root(nullptr), Panel(NxEn::GUISystem::GetPanel<AssetsBrowserPanel>())
	{
		Refresh();
	}

	AssetsBrowser::~AssetsBrowser()
	{
		Clear();
	}

	void AssetsBrowser::Clear()
	{
		Panel->Clear();
		RemoveItem(Root);
	}

	void AssetsBrowser::Refresh()
	{
		Clear();

		Root = FetchItems(NxFr::Paths::Assets, nullptr);
		Root->ImGuiText = "Assets";
		Root->Open(true);

		PurgeDuplicates(Root);

		Panel->Root = Root;
	}

	void AssetsBrowser::Create(NxFr::StringView ItemPath, NxFr::StringId Type)
	{
		NEXUS_ASSERT(!ItemPath.IsEmpty(), Default, "Can't create the Assets/ folder");

		NxFr::String TargetPath = MakeUniquePath(Validate(ItemPath));

		OnCreate(Type, TargetPath);
	}

	void AssetsBrowser::Move(NxFr::StringView ItemPath, NxFr::StringView Target)
	{
		NEXUS_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = ItemPathToId(Validate(ItemPath));
		NxFr::String TargetPath = MakeUniquePath(Validate(Target));
		AssetsBrowserItem* Item = GetItem(Id);

		DetachItem(Item);
		OnMove(Item, Item->Path, TargetPath);
		AttachItem(Item, GetParent(TargetPath));
	}

	void AssetsBrowser::Duplicate(NxFr::StringView ItemPath, NxFr::StringView Target)
	{
		NEXUS_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = ItemPathToId(Validate(ItemPath));
		NxFr::String TargetPath = MakeUniquePath(Validate(Target));
		AssetsBrowserItem* Item = GetItem(Id);

		OnDuplicate(Item, GetParent(TargetPath), Item->Path, TargetPath);
	}

	void AssetsBrowser::Delete(NxFr::StringView ItemPath)
	{
		NEXUS_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = ItemPathToId(Validate(ItemPath));
		AssetsBrowserItem* Item = GetItem(Id);

		OnDelete(Item, Item->Path);
	}

	bool AssetsBrowser::Exist(NxFr::StringView ItemPath)
	{
		return NxFr::Path::Exist(ItemPathToFsPath(Validate(ItemPath)));
	}

	NxFr::StringView AssetsBrowser::Validate(NxFr::StringView ItemPath)
	{
		if (ItemPath == NxFr::StringUtility::Empty)
		{
			return ItemPath;
		}

		if (NxFr::StringUtility::Start(ItemPath, "/"))
		{
			ItemPath = NxFr::StringUtility::TrimLeading(ItemPath, '/');
		}

		NEXUS_ASSERT(NxFr::Path::IsDirectory(ItemPath) || (NxFr::Path::IsFile(ItemPath) && NxFr::Path::HasExtension(ItemPath)), Default, "Path %s needs to have an extension or be a directory", ItemPath.C());

		return ItemPath;
	}

	NxFr::String AssetsBrowser::MakeUniquePath(NxFr::StringView ItemPath)
	{
		NxFr::StringView Name = NxFr::Path::GetName(ItemPath);

		NxFr::String Result = ItemPath;
		NxFr::String Target;
		uint64 Count = 1;

		while (Exist(Result))
		{
			Target = Name + " " + NxFr::StringUtility::ToString(Count);
			Result = NxFr::Path::ChangeName(Result, Target);
			Count++;
		}

		return Result;
	}

	AssetsBrowserItem* AssetsBrowser::FetchItems(NxFr::StringView FsPath, AssetsBrowserItem* Parent)
	{
		NxFr::String ItemPath = FsPathToItemPath(FsPath);

		AssetsBrowserItem* Item = AppendItem(ItemPath, Parent);
		UpdateItem(Item, ItemPath, true, false);

		if (Item->GetObjectType() == AssetsBrowserItemDirectory::GetClassType())
		{
			NxFr::Directory Directory(FsPath);
			for (auto& P : Directory)
			{
				FetchItems(P, Item);
			}
		}

		return Item;
	}

	AssetsBrowserItem* AssetsBrowser::PurgeDuplicates(AssetsBrowserItem* Item)
	{
		if (Item == nullptr)
		{
			return nullptr;
		}

		PurgeDuplicates(Item->Child);

		if (Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
		{
			if (Item->Next && Item->GetPrettyName() == Item->Next->GetPrettyName())
			{
				RemoveItem(Item->Next);
			}
			else if (Item->Previous && Item->GetPrettyName() == Item->Previous->GetPrettyName())
			{
				RemoveItem(Item->Previous);
			}
		}

		while (Item)
		{
			Item = PurgeDuplicates(Item->Next);
		}

		return Item ? Item->Next : nullptr;
	}

	void AssetsBrowser::UpdateItem(AssetsBrowserItem* Item, NxFr::StringView ItemPath, bool Add, bool Remove)
	{
		if (Remove && Item->Id && Items.ContainsKey(Item->Id))
		{
			Items.Remove(Item->Id);
		}

		Panel->SelectItem(Item, false, false, false);

		Item->Path = ItemPath;
		Item->Id = ItemPathToId(ItemPath);
		Item->Type = Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType() ? Assets->GetMetadata(Item->Id).GetType() : Item->GetObjectType();
		Item->CacheImGuiText();

		if (Add && Item->Id && !Items.ContainsKey(Item->Id))
		{
			Items.Append(Item->Id, Item);
		}
	}

	AssetsBrowserItem* AssetsBrowser::AppendItem(NxFr::StringView ItemPath, AssetsBrowserItem* Parent)
	{
		AssetsBrowserItem* Item = nullptr;
		if (NxFr::Path::IsDirectory(ItemPath) || ItemPath == NxFr::StringUtility::Empty)
		{
			Item = new AssetsBrowserItemDirectory();
		}
		else
		{
			if (NxFr::Path::GetExtension(ItemPath) == NxEn::AssetMetadata::AssetExtension)
			{
				Item = new AssetsBrowserItemAsset();
			}
			else
			{
				Item = new AssetsBrowserItemContent();
			}
		}

		Item->Initialize();
		Item->SetEnabled(true);

		Item->Path = ItemPath;
		AttachItem(Item, Parent);

		Panel->OnCreateItem(Item);
		return Item;
	}

	void AssetsBrowser::RemoveItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		if (Items.ContainsKey(Item->Id))
		{
			Items.Remove(Item->Id);
		}

		Item->SetEnabled(false);
		Item->Shutdown();

		DetachItem(Item);
		while (Item->Child)
		{
			RemoveItem(Item->Child);
		}

		Panel->OnDestroyItem(Item);
		delete Item;
	}

	void AssetsBrowser::AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent)
	{
		if (!Item || !Parent)
		{
			return;
		}

		Item->Parent = Parent;
		if (Parent->Child)
		{
			AssetsBrowserItem* Iterator = Parent->Child;
			while (Iterator->Next && *Iterator->Next <= *Item)
			{
				Iterator = Iterator->Next;
			}

			Item->Next = Iterator->Next;
			Item->Previous = Iterator;
			if (Iterator->Next)
			{
				Iterator->Next->Previous = Item;
			}
			Iterator->Next = Item;
		}
		else
		{
			Parent->Child = Item;

			Item->Previous = nullptr;
			Item->Next = nullptr;
		}
	}

	void AssetsBrowser::DetachItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		if (Item->Previous)
		{
			Item->Previous->Next = Item->Next;
		}
		if (Item->Next)
		{
			Item->Next->Previous = Item->Previous;
		}
		if (Item->Parent)
		{
			if (Item->Parent->Child == Item)
			{
				Item->Parent->Child = Item->Next;
			}
		}

		Item->Parent = nullptr;
		Item->Previous = nullptr;
		Item->Next = nullptr;
	}

	AssetsBrowserItem* AssetsBrowser::GetItem(NxFr::GUID Id)
	{
		AssetsBrowserItem** Item = Items.TryGet(Id);
		return Item ? *Item : nullptr;
	}

	AssetsBrowserItem* AssetsBrowser::GetParent(NxFr::StringView Path)
	{
		NxFr::StringView Directory = NxFr::Path::GetFolder(Path);
		return GetItem(ItemPathToId(Directory));
	}

	void AssetsBrowser::OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath)
	{
		NxFr::String AssetPath = ItemPathToAssetPath(TargetPath);

		AssetsBrowserItem* Item = AppendItem(AssetPath, GetParent(TargetPath));
		Item->OnCreate(ItemPathToCallbackPath(TargetPath, Item), Type);
		UpdateItem(Item, AssetPath, true, false);
	}

	void AssetsBrowser::OnMove(AssetsBrowserItem* Item, NxFr::StringView ItemPath, NxFr::StringView TargetPath)
	{
		AssetsBrowserItem* Iterator = Item->Child;
		while (Iterator)
		{
			OnMove(Iterator, Iterator->Path, NxFr::Path::Combine(TargetPath, Iterator->GetTargetName()));
			Iterator = Iterator->Next;
		}

		Item->OnMove(ItemPathToCallbackPath(ItemPath, Item), ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Item, TargetPath, true, true);
	}

	void AssetsBrowser::OnDuplicate(AssetsBrowserItem* Item, AssetsBrowserItem* Parent, NxFr::StringView ItemPath, NxFr::StringView TargetPath)
	{
		AssetsBrowserItem* Copy = AppendItem(TargetPath, Parent);

		AssetsBrowserItem* Iterator = Item->Child;
		while (Iterator)
		{
			OnDuplicate(Iterator, Copy, Iterator->Path, NxFr::Path::Combine(TargetPath, Iterator->GetTargetName()));
			Iterator = Iterator->Next;
		}

		Item->OnDuplicate(ItemPathToCallbackPath(ItemPath, Item), ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Copy, TargetPath, true, false);
	}

	void AssetsBrowser::OnDelete(AssetsBrowserItem* Item, NxFr::StringView ItemPath)
	{
		while (Item->Child)
		{
			OnDelete(Item->Child, Item->Child->Path);
		}

		Item->OnDelete(ItemPathToCallbackPath(ItemPath, Item));
		RemoveItem(Item);
	}

	NxFr::GUID AssetsBrowser::ItemPathToId(NxFr::StringView ItemPath)
	{
		if (ItemPath == NxFr::StringUtility::Empty)
		{
			return NxFr::Hash<>::HashObject(NxFr::Paths::Assets);
		}

		return NxFr::Path::GetExtension(ItemPath) == NxEn::AssetMetadata::AssetExtension ?
			Assets->PathToId(NxFr::Path::GetPathWithoutExtension(ItemPath)) : NxFr::Hash<>::HashObject(ItemPath);
	}

	NxFr::String AssetsBrowser::IdToItemPath(NxFr::GUID Id)
	{
		AssetsBrowserItem** Item = Items.TryGet(Id);
		return Item ? (*Item)->Path : NxFr::StringUtility::Empty;
	}

	NxFr::String AssetsBrowser::ItemPathToAssetPath(NxFr::StringView ItemPath)
	{
		return NxFr::Path::ChangeExtension(ItemPath, NxEn::AssetMetadata::AssetExtension);
	}

	NxFr::String AssetsBrowser::ItemPathToCallbackPath(NxFr::StringView ItemPath, AssetsBrowserItem* Item)
	{
		return Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType() ? NxFr::String(ItemPath) : ItemPathToFsPath(ItemPath);
	}

	NxFr::String AssetsBrowser::ItemPathToFsPath(NxFr::StringView ItemPath)
	{
		if (ItemPath == NxFr::StringUtility::Empty)
		{
			return NxFr::Paths::Assets;
		}

		return NxFr::Path::MakeAbsolute(ItemPath, NxFr::Paths::Assets);
	}

	NxFr::String AssetsBrowser::FsPathToItemPath(NxFr::StringView FsPath)
	{
		if (FsPath == NxFr::Paths::Assets)
		{
			return NxFr::StringUtility::Empty;
		}

		return NxFr::Path::MakeRelative(FsPath, NxFr::Paths::Assets);
	}
}
