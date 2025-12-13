#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
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
		: Parent(nullptr), Previous(nullptr), Next(nullptr), Child(nullptr),
		Id(0), Path(), ImGuiText(),
		Expanded(false), Selected(false)
	{
		SetTickable(false);
	}

	AssetsBrowserItem::~AssetsBrowserItem()
	{
	}

	void AssetsBrowserItem::Update(NxFr::StringView Target, bool UpdateId)
	{
		NxEn::GUISystem::GetPanel<AssetsBrowserPanel>()->UpdateItem(this, Target, UpdateId);
	}

	NxFr::String AssetsBrowserItem::PathToDisk(NxFr::StringView Path)
	{
		return AssetsBrowserPanel::PathToDisk(Path);
	}

	NxFr::String AssetsBrowserItem::PathToAsset(NxFr::StringView Path)
	{
		return AssetsBrowserPanel::PathToAsset(Path);
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
		NxFr::Directory(PathToDisk(Target)).Create();
		Update(Target, true);
	}

	void AssetsBrowserItemDirectory::Move(NxFr::StringView Target)
	{
		NxFr::String Before = PathToDisk(Path);
		NxFr::String After = PathToDisk(Target);

		NxFr::Directory(After).EnsureParent().Create();

		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Move(NxFr::Path::Combine(Target, Item->GetName()));
			Item = Item->GetNext();
		}

		NxFr::Directory(Before).Delete();

		Update(Target, true);
	}

	void AssetsBrowserItemDirectory::Duplicate(NxFr::StringView Target)
	{
		NxFr::Directory(PathToDisk(Target)).EnsureParent().Create();

		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Duplicate(NxFr::Path::Combine(Target, Item->GetName()));
			Item = Item->GetNext();
		}

		Update(Target, true);
	}

	void AssetsBrowserItemDirectory::Delete()
	{
		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Delete();
			Item = Item->GetNext();
		}

		NxFr::Directory(PathToDisk(Path)).Delete();
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
		NxFr::File(PathToDisk(Target)).Create();
		Update(Target, true);
	}

	void AssetsBrowserItemFile::Move(NxFr::StringView Target)
	{
		NxFr::String Before = PathToDisk(Path);
		NxFr::String After = PathToDisk(Target);

		NxFr::File(After).EnsureParent();
		NxFr::File(Before).Move(After);

		Update(Target, true);
	}

	void AssetsBrowserItemFile::Duplicate(NxFr::StringView Target)
	{
		NxFr::File(PathToDisk(Path)).Copy(PathToDisk(Target));
		Update(Target, true);
	}

	void AssetsBrowserItemFile::Delete()
	{
		NxFr::File(PathToDisk(Path)).Delete();
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
		Update(PathToAsset(Target), true);
	}

	void AssetsBrowserItemAsset::Move(NxFr::StringView Target)
	{
		Update(Target, false);
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Move(Id, GetPrettyPath());
	}

	void AssetsBrowserItemAsset::Duplicate(NxFr::StringView Target)
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Copy(Id, NxFr::Path::GetPathWithoutExtension(Target));
		Update(Target, true);
	}

	void AssetsBrowserItemAsset::Delete()
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Delete(Id);
	}

#pragma endregion

}
