#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Core/NexusEditorApplication.h"

namespace NxEd
{
	static NxEn::Command* CmdAssetBrowserCreateDirectory = NxEn::Command::Create("Assets.Browser.Create.Directory"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Item)
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser()->Create(0, Item);
	}));
	static NxEn::Command* CmdAssetBrowserCreateFile = NxEn::Command::Create("Assets.Browser.Create.File"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Item, NxFr::StringView Type)
	{
		NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser()->Create(NxFr::StringId(Type), Item);
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
		: Assets(nullptr), Edit(nullptr),
		Items(), Panel(nullptr), Context()
	{
		Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Edit = NxEn::Application::GetSystem<EditSystem>();

		Panel = NxEn::Application::GetSystem<NxEn::GUISystem>()->GetPanel<AssetsBrowserPanel>();
		Context = new AssetsBrowserEditContext(this, Panel);
		Panel->SetBrowser(this, Context);
		Edit->RegisterContext(Context);

		Panel->AppendAction(NxEn::TreeAction{ .Name = "Create", .Action = [&]() { Edit->Create(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Rename", .Action = [&]() { Edit->Rename(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Duplicate", .Action = [&]() { Edit->Duplicate(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Move", .Action = [&]() { Edit->Move(); }, .Priority = -1 });
		Panel->AppendAction(NxEn::TreeAction{ .Name = "Delete", .Action = [&]() { Edit->Delete(); }, .Priority = -1 });

		FetchItems();
	}

	AssetsBrowser::~AssetsBrowser()
	{
		ClearItems();

		Edit->UnregisterContext(Context->GetId());
		delete Context;
	}

	AssetsBrowserItem* AssetsBrowser::GetItem(NxFr::GUID Id)
	{
		AssetsBrowserItem** Item = Items.TryGet(Id);
		return Item ? *Item : nullptr;
	}

	void AssetsBrowser::Refresh()
	{
		FetchItems();
	}

	void AssetsBrowser::Create(NxFr::StringId Type, NxFr::StringView TargetPath)
	{
		NX_ASSERT(Type != NxFr::StringUtility::Id, Default, "Can't create the Asset without type");
		NX_ASSERT(!TargetPath.IsEmpty(), Default, "Can't create the Assets/ folder");

		NxFr::String Target = MakeUniquePath(Validate(TargetPath));
		AssetsBrowserItem* Parent = GetItem(PathToParent(TargetPath));

		OnCreate(Type, Target, Parent);
	}

	void AssetsBrowser::Move(NxFr::StringView ItemPath, NxFr::StringView TargetPath)
	{
		NX_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID InstanceId = ItemPathToId(Validate(ItemPath));
		NxFr::String Target = MakeUniquePath(Validate(TargetPath));
		AssetsBrowserItem* Item = GetItem(InstanceId);
		AssetsBrowserItem* Parent = GetItem(PathToParent(TargetPath));

		if (!Item)
		{
			NX_LOG(Error, System, "Failed to move asset at path %s", ItemPath.C());
			return;
		}

		OnMove(Item, Target, Parent);
	}

	void AssetsBrowser::Duplicate(NxFr::StringView ItemPath, NxFr::StringView TargetPath)
	{
		NX_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID InstanceId = ItemPathToId(Validate(ItemPath));
		NxFr::String Target = MakeUniquePath(Validate(TargetPath));
		AssetsBrowserItem* Item = GetItem(InstanceId);
		AssetsBrowserItem* Parent = GetItem(PathToParent(TargetPath));

		if (!Item)
		{
			NX_LOG(Error, System, "Failed to move asset at path %s", ItemPath.C());
			return;
		}

		OnDuplicate(Item, Target, Parent);
	}

	void AssetsBrowser::Delete(NxFr::StringView ItemPath)
	{
		NX_ASSERT(!ItemPath.IsEmpty(), Default, "Can't move the Assets/ folder");

		NxFr::GUID InstanceId = ItemPathToId(Validate(ItemPath));
		AssetsBrowserItem* Item = GetItem(InstanceId);

		if (!Item)
		{
			NX_LOG(Error, System, "Failed to move asset at path %s", ItemPath.C());
			return;
		}

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

	void AssetsBrowser::FetchItems()
	{
		ClearItems();

		AssetsBrowserItem* Root = FetchItems(NxFr::Globals::Paths::Assets, nullptr);
		Root->Label = "Assets";
		Root->Open(true);
		Panel->SetRoot(Root->GetId());

		PurgeItems(Root);
	}

	AssetsBrowserItem* AssetsBrowser::FetchItems(NxFr::StringView FsPath, AssetsBrowserItem* Parent)
	{
		NxFr::String ItemPath = FsPathToItemPath(FsPath);

		AssetsBrowserItem* Item = AppendItem(ItemPath);
		UpdateItem(Item, ItemPath);
		AttachItem(Item, Parent);

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

	AssetsBrowserItem* AssetsBrowser::PurgeItems(AssetsBrowserItem* Item)
	{
		if (Item == nullptr)
		{
			return nullptr;
		}

		PurgeItems(GetItem(Item->Child));

		if (Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
		{
			AssetsBrowserItem* Next = GetItem(Item->Next);
			AssetsBrowserItem* Previous = GetItem(Item->Previous);

			if (Next && Item->GetPrettyName() == Next->GetPrettyName())
			{
				DetachItem(Next);
				RemoveItem(Next);
			}
			else if (Previous && Item->GetPrettyName() == Previous->GetPrettyName())
			{
				DetachItem(Previous);
				RemoveItem(Previous);
			}
		}

		while (Item)
		{
			Item = PurgeItems(GetItem(Item->Next));
		}

		return Item ? GetItem(Item->Next) : nullptr;
	}

	void AssetsBrowser::ClearItems()
	{
		Panel->Clear();
		Edit->Unselect(Context->GetId());

		for (auto [InstanceId, Item] : Items)
		{
			delete Item;
		}

		Items.Clear();
	}

	AssetsBrowserItem* AssetsBrowser::AppendItem(NxFr::StringView ItemPath)
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

		return Item;
	}

	void AssetsBrowser::UpdateItem(AssetsBrowserItem* Item, NxFr::StringView ItemPath, bool UpdateId)
	{
		if (UpdateId)
		{
			Items.TryRemove(Item->GetId());
		}

		Item->Path = ItemPath;
		Item->Id = ItemPathToId(ItemPath);
		Item->Type = Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType() ? Assets->GetMetadata(Item->GetId())->GetType() : Item->GetObjectType();
		Item->Label = NxEn::GUI::Utils::NexusToImGuiId(Item->GetPrefix() + " " + Item->GetName(), Item->GetId());

		Items.TryAppend(Item->GetId(), Item);

		Panel->OnCreateItem(Item->GetId());
		Context->OnCreated.Invoke(Item->GetId());
	}

	void AssetsBrowser::RemoveItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		Panel->OnDestroyItem(Item->GetId());
		Context->OnDestroyed.Invoke(Item->GetId());

		Items.TryRemove(Item->GetId());
		delete Item;
	}

	void AssetsBrowser::AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent)
	{
		if (!Item || !Parent)
		{
			return;
		}

		Item->Parent = Parent->GetId();
		if (Parent->Child)
		{
			AssetsBrowserItem* ParentChild = GetItem(Parent->Child);
			if (Item->Compare(*ParentChild))
			{
				Item->Next = Parent->Child;
				ParentChild->Previous = Item->GetId();
				Parent->Child = Item->GetId();
			}
			else
			{
				AssetsBrowserItem* Iterator = ParentChild;
				AssetsBrowserItem* IteratorNext = GetItem(Iterator->Next);

				while (IteratorNext && IteratorNext->Compare(*Item))
				{
					Iterator = IteratorNext;
					IteratorNext = GetItem(Iterator->Next);
				}

				Item->Next = Iterator->Next;
				Item->Previous = Iterator->GetId();
				if (IteratorNext)
				{
					IteratorNext->Previous = Item->GetId();
				}
				Iterator->Next = Item->GetId();
			}
		}
		else
		{
			Parent->Child = Item->GetId();

			Item->Previous = 0;
			Item->Next = 0;
		}
	}

	void AssetsBrowser::DetachItem(AssetsBrowserItem* Item)
	{
		if (!Item)
		{
			return;
		}

		AssetsBrowserItem* Previous = GetItem(Item->Previous);
		AssetsBrowserItem* Next = GetItem(Item->Next);
		AssetsBrowserItem* Parent = GetItem(Item->Parent);

		if (Previous)
		{
			Previous->Next = Item->Next;
		}
		if (Next)
		{
			Next->Previous = Item->Previous;
		}
		if (Parent)
		{
			if (Parent->Child == Item->GetId())
			{
				Parent->Child = Item->Next;
			}
		}

		Item->Parent = 0;
		Item->Previous = 0;
		Item->Next = 0;
	}

	void AssetsBrowser::OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath, AssetsBrowserItem* Parent)
	{
		NxFr::String ItemPath = ItemPathToAssetPath(TargetPath);

		AssetsBrowserItem* Item = AppendItem(ItemPath);
		Item->OnCreate(Type, ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Item, ItemPath);
		AttachItem(Item, Parent);
	}

	void AssetsBrowser::OnMove(AssetsBrowserItem* Item, NxFr::StringView TargetPath, AssetsBrowserItem* Parent)
	{
		AssetsBrowserItem* Iterator = GetItem(Item->Child);
		while (Iterator)
		{
			OnMove(Iterator, NxFr::Path::ChangeFolder(Iterator->GetTargetPath(), TargetPath), Item);
			Iterator = GetItem(Iterator->Next);
		}

		DetachItem(Item);
		Item->OnMove(ItemPathToCallbackPath(Item->Path, Item), ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Item, TargetPath, true);
		AttachItem(Item, Parent);
	}

	void AssetsBrowser::OnDuplicate(AssetsBrowserItem* Item, NxFr::StringView TargetPath, AssetsBrowserItem* Parent)
	{
		AssetsBrowserItem* Copy = AppendItem(TargetPath);

		AssetsBrowserItem* Iterator = GetItem(Item->Child);
		while (Iterator)
		{
			OnDuplicate(Iterator, NxFr::Path::ChangeFolder(Iterator->GetTargetPath(), TargetPath), Copy);
			Iterator = GetItem(Iterator->Next);
		}

		Item->OnDuplicate(ItemPathToCallbackPath(Item->Path, Item), ItemPathToCallbackPath(TargetPath, Item));
		UpdateItem(Copy, TargetPath);
		AttachItem(Copy, Parent);
	}

	void AssetsBrowser::OnDelete(AssetsBrowserItem* Item)
	{
		while (Item->Child)
		{
			OnDelete(GetItem(Item->Child));
		}

		Item->OnDelete(ItemPathToCallbackPath(Item->Path, Item));
		DetachItem(Item);
		RemoveItem(Item);
	}

	NxFr::GUID AssetsBrowser::PathToParent(NxFr::StringView Path)
	{
		NxFr::StringView Directory = NxFr::Path::GetFolder(Path, true);
		return ItemPathToId(Directory);
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
		if (NxFr::Path::IsDirectory(ItemPath))
		{
			return ItemPath;
		}

		return NxFr::Path::ChangeExtension(ItemPath, NxEn::AssetMetadata::AssetExtension);
	}

	NxFr::String AssetsBrowser::ItemPathToCallbackPath(NxFr::StringView ItemPath, AssetsBrowserItem* Item)
	{
		return Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType() ? NxFr::String(ItemPath) : ItemPathToFsPath(ItemPath);
	}
}
