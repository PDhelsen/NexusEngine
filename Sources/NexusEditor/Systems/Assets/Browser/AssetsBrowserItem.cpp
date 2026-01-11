#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserInternal.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"

#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItem)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemDirectory)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemFile)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemAsset)

#pragma region AssetsBrowserItem

	AssetsBrowserItem::AssetsBrowserItem()
		: TreeItem(),
		Id(0), Path(""), Type(0),
		Parent(nullptr), Previous(nullptr), Next(nullptr), Child(nullptr)
	{
	}

	AssetsBrowserItem::~AssetsBrowserItem()
	{
	}

	AssetsBrowserItem* AssetsBrowserItem::GetParent() const
	{
		return Parent;
	}

	void AssetsBrowserItem::SetParent(TreeItem* Instance)
	{
		Parent = static_cast<AssetsBrowserItem*>(Instance);
	}

	AssetsBrowserItem* AssetsBrowserItem::GetPrevious() const
	{
		return Previous;
	}

	void AssetsBrowserItem::SetPrevious(TreeItem* Instance)
	{
		Previous = static_cast<AssetsBrowserItem*>(Instance);
	}

	AssetsBrowserItem* AssetsBrowserItem::GetNext() const
	{
		return Next;
	}

	void AssetsBrowserItem::SetNext(TreeItem* Instance)
	{
		Next = static_cast<AssetsBrowserItem*>(Instance);
	}

	AssetsBrowserItem* AssetsBrowserItem::GetChild() const
	{
		return Child;
	}

	void AssetsBrowserItem::SetChild(TreeItem* Instance)
	{
		Child = static_cast<AssetsBrowserItem*>(Instance);
	}

	void AssetsBrowserItem::GenerateImGuiText()
	{
		ImGuiText = GetPrefix() + " " + GetPrettyName() + "##" + NxFr::StringUtility::ToString(Id);
	}

	int8 AssetsBrowserItem::Compare(const TreeItem& Other) const
	{
		return Path < static_cast<const AssetsBrowserItem&>(Other).Path;
	}

	void AssetsBrowserItem::ForceImGuiText(NxFr::StringView Text)
	{
		ImGuiText = Text;
	}

	void AssetsBrowserItem::Update(NxFr::GUID Id, NxFr::StringView Path, NxFr::StringId Type)
	{
		this->Id = Id;
		this->Path = Path;
		this->Type = Type;

		GenerateImGuiText();
	}

#pragma endregion

#pragma region AssetsBrowserItemDirectory

	AssetsBrowserItemDirectory::AssetsBrowserItemDirectory()
	{
	}

	AssetsBrowserItemDirectory::~AssetsBrowserItemDirectory()
	{
	}

	void AssetsBrowserItemDirectory::Create(NxFr::StringView Target, NxFr::StringId Type)
	{
		NxFr::Directory(AssetsBrowser::PathToDisk(Target)).Create();
		AssetsBrowser::UpdateItem(this, Target, true, false);
	}

	void AssetsBrowserItemDirectory::Move(NxFr::StringView Target)
	{
		NxFr::String Before = AssetsBrowser::PathToDisk(GetPath());
		NxFr::String After = AssetsBrowser::PathToDisk(Target);

		NxFr::Directory(After).EnsureParent().Create();

		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Move(NxFr::Path::Combine(Target, Item->GetName()));
			Item = Item->GetNext();
		}

		NxFr::Directory(Before).Delete();

		AssetsBrowser::UpdateItem(this, Target, true, true);
	}

	void AssetsBrowserItemDirectory::Duplicate(NxFr::StringView Target)
	{
		NxFr::Directory(AssetsBrowser::PathToDisk(Target)).EnsureParent().Create();

		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Duplicate(NxFr::Path::Combine(Target, Item->GetName()));
			Item = Item->GetNext();
		}

		AssetsBrowser::UpdateItem(this, Target, true, false);
	}

	void AssetsBrowserItemDirectory::Delete()
	{
		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Delete();
			Item = Item->GetNext();
		}

		NxFr::Directory(AssetsBrowser::PathToDisk(GetPath())).Delete();
	}

#pragma endregion

#pragma region AssetsBrowserItemFile

	AssetsBrowserItemFile::AssetsBrowserItemFile()
	{
	}

	AssetsBrowserItemFile::~AssetsBrowserItemFile()
	{
	}

	void AssetsBrowserItemFile::Create(NxFr::StringView Target, NxFr::StringId Type)
	{
		NxFr::File(AssetsBrowser::PathToDisk(Target)).Create();
		AssetsBrowser::UpdateItem(this, Target, true, false);
	}

	void AssetsBrowserItemFile::Move(NxFr::StringView Target)
	{
		NxFr::String Before = AssetsBrowser::PathToDisk(GetPath());
		NxFr::String After = AssetsBrowser::PathToDisk(Target);

		NxFr::File(After).EnsureParent();
		NxFr::File(Before).Move(After);

		AssetsBrowser::UpdateItem(this, Target, true, true);
	}

	void AssetsBrowserItemFile::Duplicate(NxFr::StringView Target)
	{
		NxFr::File(AssetsBrowser::PathToDisk(GetPath())).Copy(AssetsBrowser::PathToDisk(Target));
		AssetsBrowser::UpdateItem(this, Target, true, false);
	}

	void AssetsBrowserItemFile::Delete()
	{
		NxFr::File(AssetsBrowser::PathToDisk(GetPath())).Delete();
	}

#pragma endregion

#pragma region AssetsBrowserItemAsset

	AssetsBrowserItemAsset::AssetsBrowserItemAsset()
	{
	}

	AssetsBrowserItemAsset::~AssetsBrowserItemAsset()
	{
	}

	void AssetsBrowserItemAsset::Create(NxFr::StringView Target, NxFr::StringId Type)
	{
		NxEn::AssetsSystem* System = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxFr::StringView AssetPath = NxFr::Path::GetPathWithoutExtension(Target);
		NxFr::StringView Extension = NxFr::Path::GetExtension(Target);

		NxEn::Asset* Instance = System->Create(Type, AssetPath, Extension);
		AssetsBrowser::UpdateItem(this, AssetsBrowser::PathToAsset(Target), true, false);
	}

	void AssetsBrowserItemAsset::Move(NxFr::StringView Target)
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Move(GetId(), NxFr::Path::GetPathWithoutExtension(Target));
		AssetsBrowser::UpdateItem(this, Target, false, false);
	}

	void AssetsBrowserItemAsset::Duplicate(NxFr::StringView Target)
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Copy(GetId(), NxFr::Path::GetPathWithoutExtension(Target));
		AssetsBrowser::UpdateItem(this, Target, true, false);
	}

	void AssetsBrowserItemAsset::Delete()
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Delete(GetId());
	}

#pragma endregion

}
