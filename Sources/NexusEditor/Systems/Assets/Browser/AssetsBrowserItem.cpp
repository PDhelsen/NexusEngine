#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserPanel.h"
#include "NexusEditor/Systems/Assets/Importers/AssetImporter.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItem)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemDirectory)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemFile)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemAsset)

	NxFr::Delegate<NxFr::String(NxFr::StringView)> AssetsBrowserItem::PathToFile;
	NxFr::Delegate<void(AssetsBrowserItem*, NxFr::StringView, bool)> AssetsBrowserItem::Update;

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

	NxFr::String AssetsBrowserItem::GetFilePath()
	{
		return PathToFile.Invoke(Path);
	}

	void AssetsBrowserItem::ChangeFilePath(NxFr::StringView FilePath, NxFr::String& Before, NxFr::String& After)
	{
		Before = PathToFile.Invoke(Path);
		Update.Invoke(this, FilePath, true);
		After = PathToFile.Invoke(Path);
	}

#pragma endregion

#pragma region AssetsBrowserItemDirectory

	AssetsBrowserItemDirectory::AssetsBrowserItemDirectory()
	{
	}

	AssetsBrowserItemDirectory::~AssetsBrowserItemDirectory()
	{
	}

	void AssetsBrowserItemDirectory::Create(NxFr::StringView FilePath, NxFr::StringId Type)
	{
	}

	void AssetsBrowserItemDirectory::Move(NxFr::StringView FilePath)
	{
		NxFr::String Before, After;
		ChangeFilePath(FilePath, Before, After);

		NxFr::Directory(After).EnsureParent().Create();

		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Move(NxFr::Path::Combine(FilePath, Item->GetName()));
			Item = Item->GetNext();
		}

		NxFr::Directory(Before).Delete();
	}

	void AssetsBrowserItemDirectory::Delete()
	{
		AssetsBrowserItem* Item = GetChild();
		while (Item)
		{
			Item->Delete();
			Item = Item->GetNext();
		}

		NxFr::Directory(GetFilePath()).Delete();
	}

#pragma endregion

#pragma region AssetsBrowserItemFile

	AssetsBrowserItemFile::AssetsBrowserItemFile()
	{
	}

	AssetsBrowserItemFile::~AssetsBrowserItemFile()
	{
	}

	void AssetsBrowserItemFile::Create(NxFr::StringView FilePath, NxFr::StringId Type)
	{
		NEXUS_ASSERT(false, Default, "Unsupported");
	}

	void AssetsBrowserItemFile::Move(NxFr::StringView FilePath)
	{
		NxFr::String Before, After;
		ChangeFilePath(FilePath, Before, After);

		NxFr::File(After).EnsureParent();
		NxFr::File(Before).Move(After);
	}

	void AssetsBrowserItemFile::Delete()
	{
		NxFr::File(GetFilePath()).Delete();
	}

#pragma endregion

#pragma region AssetsBrowserItemAsset

	AssetsBrowserItemAsset::AssetsBrowserItemAsset()
	{
	}

	AssetsBrowserItemAsset::~AssetsBrowserItemAsset()
	{
	}

	void AssetsBrowserItemAsset::Create(NxFr::StringView FilePath, NxFr::StringId Type)
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Create(Type,
			NxFr::Path::GetPathWithoutExtension(FilePath),
			NxFr::Path::GetExtension(FilePath));
	}

	void AssetsBrowserItemAsset::Move(NxFr::StringView FilePath)
	{
		Update.Invoke(this, FilePath, false);
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Move(Id, GetPrettyPath());
	}

	void AssetsBrowserItemAsset::Delete()
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Delete(Id);
	}

#pragma endregion

}
