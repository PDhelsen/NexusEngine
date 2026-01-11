#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserInternal.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxEd
{
	const static NxEn::Command CmdAssetCreateDirectory = NxEn::Command::Create("Assets.Create.Directory"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsBrowser::Create(Path, 0);
	}));
	const static NxEn::Command CmdAssetCreateFile = NxEn::Command::Create("Assets.Create.File"_Sid, "Create at path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Type)
	{
		AssetsBrowser::Create(Path, NxFr::StringId(Type));
	}));
	const static NxEn::Command CmdAssetMove = NxEn::Command::Create("Assets.Move"_Sid, "Move path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		AssetsBrowser::Move(Path, Target);
	}));
	const static NxEn::Command CmdAssetDuplicate = NxEn::Command::Create("Assets.Duplicate"_Sid, "Duplicate path in the browser", NxFr::Delegate<void(NxFr::StringView, NxFr::StringView)>([](NxFr::StringView Path, NxFr::StringView Target)
	{
		AssetsBrowser::Duplicate(Path, Target);
	}));
	const static NxEn::Command CmdAssetDelete = NxEn::Command::Create("Assets.Delete"_Sid, "Delete path in the browser", NxFr::Delegate<void(NxFr::StringView)>([](NxFr::StringView Path)
	{
		AssetsBrowser::Delete(Path);
	}));

	namespace AssetsBrowser
	{
		static NxFr::Dictionary<NxFr::GUID, AssetsBrowserItem*> Items;

		static NxEn::AssetsSystem* GetSystem()
		{
			return NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		}

		const NxFr::String Root = NxFr::String(NxFr::Paths::Assets);
		const NxFr::String RootPath = NxFr::StringUtility::Empty;
		const NxFr::String RootLabel = "Assets";

		void Create(NxFr::StringView Path, NxFr::StringId Type)
		{
			NEXUS_ASSERT(Path != RootPath, Default, "Can't create the Assets/ folder");

			NxFr::String AssetPath = PathToAsset(EnsureUniquePath(ValidatePath(Path)));

			AssetsBrowserItem* Item = AppendItem(AssetPath);
			Item->Create(Path, Type);
			AttachItem(Item, GetParent(AssetPath));
			SortItem(Item);
		}

		void Move(NxFr::StringView Path, NxFr::StringView Target)
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

		void Duplicate(NxFr::StringView Path, NxFr::StringView Target)
		{
			NEXUS_ASSERT(Path != RootPath, Default, "Can't duplicate the Assets/ folder");

			NxFr::GUID Id = PathToId(ValidatePath(Path));

			AssetsBrowserItem* Item = GetItem(Id);
			Item = DuplicateItem(Item, nullptr);
			Item->Duplicate(EnsureUniquePath(ValidatePath(Target)));
			AttachItem(Item, GetParent(Item->GetPath()));
			SortItem(Item);
		}

		void Delete(NxFr::StringView Path)
		{
			NEXUS_ASSERT(Path != RootPath, Default, "Can't delete the Assets/ folder");

			NxFr::GUID Id = PathToId(ValidatePath(Path));

			AssetsBrowserItem* Item = GetItem(Id);
			Item->Delete();
			DetachItem(Item);
			RemoveItem(Item);
			SortItem(Item);
		}

		bool Exist(NxFr::StringView Path)
		{
			return NxFr::Path::Exist(PathToDisk(ValidatePath(Path)));
		}

		NxFr::StringView ValidatePath(NxFr::StringView Path)
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

		NxFr::String EnsureUniquePath(NxFr::String Path)
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

		AssetsBrowserItem* GetItem(NxFr::GUID Id)
		{
			AssetsBrowserItem** Item = Items.TryGet(Id);
			return Item ? *Item : nullptr;
		}

		AssetsBrowserItem* GetParent(NxFr::StringView Path)
		{
			NxFr::StringView Directory = NxFr::Path::GetParent(Path);
			if (Directory.IsEmpty())
			{
				Directory = RootPath;
			}

			NxFr::GUID DirectoryId = PathToId(Directory);
			return GetItem(DirectoryId);
		}

		AssetsBrowserItem* FetchItems()
		{
			AssetsBrowserItem* Item = FetchItems(Root, nullptr);
			Item->ForceImGuiText(RootLabel);
			Item->Open(true);

			PurgeDuplicates(Item);
			return Item;
		}

		AssetsBrowserItem* FetchItems(NxFr::StringView Path, AssetsBrowserItem* Parent)
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

		AssetsBrowserItem* PurgeDuplicates(AssetsBrowserItem* Item)
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

		AssetsBrowserItem* AppendItem(NxFr::StringView Path)
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

			Item->Initialize();
			Item->SetEnabled(true);
			Items.Append(Item->GetId(), Item);

			return Item;
		}

		void RemoveItem(AssetsBrowserItem* Item)
		{
			if (!Item || !Items.ContainsKey(Item->GetId()))
			{
				return;
			}

			Item->SetEnabled(false);
			Item->Shutdown();

			DetachItem(Item);
			while (Item->GetChild())
			{
				RemoveItem(Item->GetChild());
			}

			Items.Remove(Item->GetId());
			delete Item;
		}

		void UpdateItem(AssetsBrowserItem* Item, NxFr::StringView Path, bool AddId, bool RemoveId)
		{
			if (RemoveId)
			{
				if (Items.ContainsKey(Item->GetId()))
				{
					Items.Remove(Item->GetId());
				}
			}

			NxFr::StringId Type = Item->GetObjectType() == AssetsBrowserItemAsset::GetClassType() ?
				GetSystem()->GetMetadata(Item->GetId()).GetType() : Item->GetObjectType();
			Item->Update(AssetsBrowser::PathToId(Path), Path, Type);

			if (AddId)
			{
				Items.Append(Item->GetId(), Item);
			}
		}

		AssetsBrowserItem* DuplicateItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent)
		{
			if (!Item)
			{
				return nullptr;
			}

			AssetsBrowserItem* Copy = AppendItem(Item->GetPath());
			UpdateItem(Copy, Item->GetPath(), false, false);

			Copy->SetParent(Parent);
			if (Item->GetChild())
			{
				Copy->SetChild(DuplicateItem(Item->GetChild(), Copy));
			}
			if (Item->GetNext() && Parent)
			{
				Copy->SetNext(DuplicateItem(Item->GetNext(), Parent));
				Copy->GetNext()->SetPrevious(Copy);
			}

			return Copy;
		}

		void AttachItem(AssetsBrowserItem* Item, AssetsBrowserItem* Parent)
		{
			if (!Item || !Parent)
			{
				return;
			}

			Item->SetParent(Parent);

			if (Parent->GetChild())
			{
				AssetsBrowserItem* Iterator = Parent->GetChild();
				while (Iterator->GetNext())
				{
					Iterator = Iterator->GetNext();
				}

				Iterator->SetNext(Item);
				Item->SetPrevious(Iterator);
				Item->SetNext(nullptr);
			}
			else
			{
				Parent->SetChild(Item);

				Item->SetPrevious(nullptr);
				Item->SetNext(nullptr);
			}
		}

		void DetachItem(AssetsBrowserItem* Item)
		{
			if (!Item)
			{
				return;
			}

			if (Item->GetPrevious())
			{
				Item->GetPrevious()->SetNext(Item->GetNext());
			}
			if (Item->GetNext())
			{
				Item->GetNext()->SetPrevious(Item->GetPrevious());
			}
			if (Item->GetParent())
			{
				if (Item->GetParent()->GetChild() == Item)
				{
					Item->GetParent()->SetChild(Item->GetNext());
				}
			}
		}

		void SortItem(AssetsBrowserItem* Item, bool Recursive)
		{
			NxFr::Delegate<AssetsBrowserItem* (AssetsBrowserItem*)> Split = [](AssetsBrowserItem* Head)
			{
				AssetsBrowserItem* Slow = Head;
				AssetsBrowserItem* Fast = Head->GetNext();

				while (Fast && Fast->GetNext())
				{
					Slow = Slow->GetNext();
					Fast = Fast->GetNext()->GetNext();
				}

				AssetsBrowserItem* Second = Slow->GetNext();
				Slow->SetNext(nullptr);
				return Second;
			};
			NxFr::Delegate<AssetsBrowserItem* (AssetsBrowserItem*, AssetsBrowserItem*)> Merge = [&](AssetsBrowserItem* A, AssetsBrowserItem* B)
			{
				if (!A) return B;
				if (!B) return A;

				if (*A < *B)
				{
					A->SetNext(Merge(A->GetNext(), B));
					if (A->GetNext())
						A->GetNext()->SetPrevious(A);
					A->SetPrevious(nullptr);
					return A;
				}
				else
				{
					B->SetNext(Merge(A, B->GetNext()));
					if (B->GetNext())
						B->GetNext()->SetPrevious(B);
					B->SetPrevious(nullptr);
					return B;
				}
			};
			NxFr::Delegate<AssetsBrowserItem* (AssetsBrowserItem*)> MergeSort = [&](AssetsBrowserItem* Item)
			{
				if (!Item || !Item->GetNext())
					return Item;

				AssetsBrowserItem* Second = Split(Item);

				Item = MergeSort(Item);
				Second = MergeSort(Second);

				return Merge(Item, Second);
			};

			Item->SetChild(MergeSort.Invoke(Item->GetChild()));

			if (Recursive)
			{
				AssetsBrowserItem* Iterator = Item->GetChild();
				while (Iterator)
				{
					SortItem(Iterator, Recursive);
					Iterator = Iterator->GetNext();
				}
			}
		}

		NxFr::GUID PathToId(NxFr::StringView Path)
		{
			if (Path == RootPath)
			{
				return NxFr::Hash<>::HashObject(RootLabel);
			}

			return NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::AssetExtension) ?
				GetSystem()->PathToId(NxFr::Path::GetPathWithoutExtension(Path)) :
				NxFr::Hash<>::HashObject(Path);
		}

		NxFr::StringView IdToPath(NxFr::GUID Id)
		{
			AssetsBrowserItem** Item = Items.TryGet(Id);
			return Item ? (*Item)->GetPath() : NxFr::StringView(RootPath);
		}

		NxFr::String DiskToPath(NxFr::StringView Path)
		{
			if (Path == NxFr::Paths::Assets)
			{
				return RootPath;
			}

			return NxFr::Path::ConvertAbsoluteToRelative(Path, NxFr::Paths::Assets);
		}

		NxFr::String PathToDisk(NxFr::StringView Path)
		{
			if (Path == RootPath)
			{
				return NxFr::Paths::Assets.Data;
			}

			return NxFr::Path::ConvertRelativeToAbsolute(Path, NxFr::Paths::Assets);
		}

		NxFr::String PathToAsset(NxFr::StringView Path)
		{
			return NxFr::Path::ChangeExtension(Path, NxEn::AssetMetadata::AssetExtension);
		}
	}
}
