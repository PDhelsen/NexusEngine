#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	static NxEn::Command* CmdAssetBrowserCreateDirectory = NxEn::Command::Create("Assets.Browser.Create.Directory"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Item)
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser()->Create(Item, 0);
	}));
	static NxEn::Command* CmdAssetBrowserCreateFile = NxEn::Command::Create("Assets.Browser.Create.File"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Item, NxFr::StringView Type)
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser()->Create(Item, NxFr::StringId(Type));
	}));
	static NxEn::Command* CmdAssetBrowserMove = NxEn::Command::Create("Assets.Browser.Move"_Sid, "Move path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Item, NxFr::StringView Target)
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser()->Move(Item, Target);
	}));
	static NxEn::Command* CmdAssetBrowserDuplicate = NxEn::Command::Create("Assets.Browser.Duplicate"_Sid, "Duplicate path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Item, NxFr::StringView Target)
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser()->Duplicate(Item, Target);
	}));
	static NxEn::Command* CmdAssetBrowserDelete = NxEn::Command::Create("Assets.Browser.Delete"_Sid, "Delete path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Item)
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser()->Delete(Item);
	}));

	AssetsBrowser::AssetsBrowser()
		: OnItemCreated(), OnItemDestroyed(), OnItemSelected(),
		Assets(nullptr), Edit(nullptr),
		Items(), Root(nullptr), Context(ContextId, this)
	{
		OnItemDestroyed += [&](AssetsBrowserItem* Item)
		{
			Edit->Unselect(Item->GetItemId(), Context.GetId());
		};
		OnItemSelected += [&](AssetsBrowserItem* Item, bool State)
		{
			if (State)
			{
				Edit->Select(Item->GetItemId(), Context.GetId());
			}
			else
			{
				Edit->Unselect(Item->GetItemId(), Context.GetId());
			}
		};

		Context.GetOnSelectionChanged() += [&](NxFr::GUID Id, bool State)
		{
			AssetsBrowserItem* Item = GetItem(Id);
			OnItemSelected.Invoke(Item, State);
		};
	}

	AssetsBrowser::~AssetsBrowser()
	{

	}

	void AssetsBrowser::Clear()
	{
		RemoveItem(Root);

		if (Edit && Edit->GetContext(ContextId))
		{
			Edit->UnregisterContext(ContextId);
		}
	}

	void AssetsBrowser::Refresh()
	{
		Clear();

		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		Root = FetchItems(NxFr::Globals::Paths::Assets, nullptr);
		PurgeDuplicates(Root);

		Edit->RegisterContext(ContextId, &Context);
		Root->ImGuiText = RootFolderName;
		Root->Open(true);
	}

	void AssetsBrowser::Create(NxFr::StringView ItemPath, NxFr::StringId Type)
	{
		NX_ASSERT(!ItemPath.IsEmpty(), Default, "Can't create the Assets/ folder");

		NxFr::String TargetPath = MakeUniquePath(Validate(ItemPath));

		OnCreate(Type, TargetPath);
	}

	void AssetsBrowser::Move(NxFr::StringView ItemPath, NxFr::StringView TargetPath)
	{
		NX_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = ItemPathToId(Validate(ItemPath));
		NxFr::String Target = MakeUniquePath(Validate(TargetPath));
		AssetsBrowserItem* Item = GetItem(Id);

		DetachItem(Item);
		OnMove(Item, Target);
		AttachItem(Item, GetParent(Target));
	}

	void AssetsBrowser::Duplicate(NxFr::StringView ItemPath, NxFr::StringView TargetPath)
	{
		NX_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = ItemPathToId(Validate(ItemPath));
		NxFr::String Target = MakeUniquePath(Validate(TargetPath));
		AssetsBrowserItem* Item = GetItem(Id);

		OnDuplicate(Item, Target);
	}

	void AssetsBrowser::Delete(NxFr::StringView ItemPath)
	{
		NX_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID Id = ItemPathToId(Validate(ItemPath));
		AssetsBrowserItem* Item = GetItem(Id);

		OnDelete(Item);
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

		NX_ASSERT(NxFr::Path::IsDirectory(ItemPath) || (NxFr::Path::IsFile(ItemPath) && NxFr::Path::HasExtension(ItemPath)), Default, "Path %s needs to have an extension or be a directory", ItemPath.C());

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
			NxFr::List<NxFr::String> Content = Directory.GetContent();
			for (auto& Path : Content)
			{
				FetchItems(Path, Item);
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

		Item->Path = ItemPath;
		AttachItem(Item, Parent);
		OnItemCreated.Invoke(Item);

		return Item;
	}

	void AssetsBrowser::UpdateItem(AssetsBrowserItem* Item, NxFr::StringView ItemPath, bool Add, bool Remove)
	{
		if (Remove && Item->Id)
		{
			Items.TryRemove(Item->Id);
		}

		Item->Path = ItemPath;
		Item->Id = ItemPathToId(ItemPath);
		Item->Type = Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType() ? Assets->GetMetadata(Item->Id).GetType() : Item->GetObjectType();
		Item->CacheImGuiText();

		OnItemSelected.Invoke(Item, true);

		if (Add && Item->Id)
		{
			Items.TryAppend(Item->Id, Item);
		}
	}

	void AssetsBrowser::RemoveItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		DetachItem(Item);
		while (Item->Child)
		{
			RemoveItem(Item->Child);
		}

		OnItemDestroyed.Invoke(Item);
		Items.TryRemove(Item->Id);
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

	void AssetsBrowser::OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath)
	{
		NxFr::String AssetPath = ItemPathToAssetPath(TargetPath);

		AssetsBrowserItem* Item = AppendItem(AssetPath, GetParent(AssetPath));
		Item->OnCreate(Type, ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Item, AssetPath, true, false);
	}

	void AssetsBrowser::OnMove(AssetsBrowserItem* Item, NxFr::StringView TargetPath)
	{
		AssetsBrowserItem* Iterator = Item->Child;
		while (Iterator)
		{
			OnMove(Iterator, NxFr::Path::Combine(TargetPath, Iterator->GetTargetName()));
			Iterator = Iterator->Next;
		}

		Item->OnMove(ItemPathToCallbackPath(Item->Path, Item), ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Item, TargetPath, true, true);
	}

	void AssetsBrowser::OnDuplicate(AssetsBrowserItem* Item, NxFr::StringView TargetPath)
	{
		AssetsBrowserItem* Copy = AppendItem(TargetPath, GetParent(TargetPath));

		AssetsBrowserItem* Iterator = Item->Child;
		while (Iterator)
		{
			OnDuplicate(Iterator, NxFr::Path::Combine(TargetPath, Iterator->GetTargetName()));
			Iterator = Iterator->Next;
		}

		Item->OnDuplicate(ItemPathToCallbackPath(Item->Path, Item), ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Copy, TargetPath, true, false);
	}

	void AssetsBrowser::OnDelete(AssetsBrowserItem* Item)
	{
		while (Item->Child)
		{
			OnDelete(Item->Child);
		}

		Item->OnDelete(ItemPathToCallbackPath(Item->Path, Item));
		RemoveItem(Item);
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

	NxFr::GUID AssetsBrowser::ItemPathToId(NxFr::StringView ItemPath)
	{
		if (ItemPath == NxFr::StringUtility::Empty)
		{
			return NxFr::Hash<>::HashObject(NxFr::Globals::Paths::Assets);
		}

		return NxFr::Path::GetExtension(ItemPath) == NxEn::AssetMetadata::AssetExtension ?
			Assets->PathToId(NxFr::Path::GetPathWithoutExtension(ItemPath)) : NxFr::Hash<>::HashObject(ItemPath);
	}

	NxFr::String AssetsBrowser::ItemPathToFsPath(NxFr::StringView ItemPath)
	{
		if (ItemPath == NxFr::StringUtility::Empty)
		{
			return NxFr::Globals::Paths::Assets;
		}

		return NxFr::Path::MakeAbsolute(ItemPath, NxFr::Globals::Paths::Assets);
	}

	NxFr::String AssetsBrowser::FsPathToItemPath(NxFr::StringView FsPath)
	{
		if (FsPath == NxFr::Globals::Paths::Assets)
		{
			return NxFr::StringUtility::Empty;
		}

		return NxFr::Path::MakeRelative(FsPath, NxFr::Globals::Paths::Assets);
	}

	NxFr::String AssetsBrowser::ItemPathToAssetPath(NxFr::StringView ItemPath)
	{
		return NxFr::Path::ChangeExtension(ItemPath, NxEn::AssetMetadata::AssetExtension);
	}

	NxFr::String AssetsBrowser::ItemPathToCallbackPath(NxFr::StringView ItemPath, AssetsBrowserItem* Item)
	{
		return Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType() ? NxFr::String(ItemPath) : ItemPathToFsPath(ItemPath);
	}
}
