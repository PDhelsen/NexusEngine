#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItem)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemDirectory)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemFile)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemAsset)

#pragma region AssetsBrowserItem

	AssetsBrowserItem::AssetsBrowserItem(NxFr::GUID ItemId, NxFr::StringView FilePath)
		: Parent(nullptr), Previous(nullptr), Next(nullptr), Child(nullptr),
		Id(ItemId), Path(FilePath), ImGuiText(),
		PrettyPath(), Directory(), Name(), Extension(),
		Expanded(false), Selected(false)
	{
		SetTickable(false);

		GenerateInfo();
	}

	AssetsBrowserItem::~AssetsBrowserItem()
	{
	}

	void AssetsBrowserItem::Update(NxFr::GUID ItemId, NxFr::StringView FilePath)
	{
		Id = ItemId;
		Path = FilePath;

		GenerateInfo();
		GenerateImGui();
	}

	void AssetsBrowserItem::GenerateInfo()
	{
		PrettyPath = NxFr::Path::GetPathWithoutExtension(Path);
		Directory = NxFr::Path::GetDirectoryPath(Path);
		Name = NxFr::Path::IsDirectory(Path) ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path);
		Extension = NxFr::Path::GetExtension(Path);
	}

#pragma endregion

#pragma region AssetsBrowserItemDirectory

	AssetsBrowserItemDirectory::AssetsBrowserItemDirectory(NxFr::GUID ItemId, NxFr::StringView FilePath)
		: AssetsBrowserItem(ItemId, FilePath)
	{
		GenerateImGui();
	}

	AssetsBrowserItemDirectory::~AssetsBrowserItemDirectory()
	{
	}

	void AssetsBrowserItemDirectory::GenerateImGui()
	{
		ImGuiText = "D " + Name + "##" + NxFr::StringUtility::ToString(Id);
	}

	AssetsBrowserItem* AssetsBrowserItem::GetIterator()
	{
		if (Child)
		{
			return Child;
		}

		if (Next)
		{
			return Next;
		}

		AssetsBrowserItem* P = Parent;
		while (P && !P->Next)
		{
			P = P->Parent;
		}

		if (P)
		{
			return P->Next;
		}

		return nullptr;
	}

#pragma endregion

#pragma region AssetsBrowserItemFile

	AssetsBrowserItemFile::AssetsBrowserItemFile(NxFr::GUID ItemId, NxFr::StringView FilePath)
		: AssetsBrowserItem(ItemId, FilePath)
	{
		GenerateImGui();
	}

	AssetsBrowserItemFile::~AssetsBrowserItemFile()
	{
	}

	void AssetsBrowserItemFile::GenerateImGui()
	{
		ImGuiText = "F " + Name + "##" + NxFr::StringUtility::ToString(Id);
	}

#pragma endregion

#pragma region AssetsBrowserItemAsset

	AssetsBrowserItemAsset::AssetsBrowserItemAsset(NxFr::GUID ItemId, NxFr::StringView FilePath)
		: AssetsBrowserItem(ItemId, FilePath)
	{
		GenerateImGui();
	}

	AssetsBrowserItemAsset::~AssetsBrowserItemAsset()
	{
	}

	void AssetsBrowserItemAsset::GenerateImGui()
	{
		ImGuiText = "A " + Name + "##" + NxFr::StringUtility::ToString(Id);
	}

#pragma endregion

}
