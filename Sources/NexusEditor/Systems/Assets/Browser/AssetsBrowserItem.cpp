#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItem)

	AssetsBrowserItem::AssetsBrowserItem(NxFr::GUID Id, NxFr::StringView FilePath)
		: Id(Id), ItemType(), Path(FilePath), PathWithoutExtension(), Directory(), Name(), Extension(), ImGuiText(), Depth(), Parent(-1), Next(-1), Expanded(false), Selected(false)
	{
		SetTickable(false);

		ItemType = NxFr::Path::IsDirectory(Path) ? Type::Directory :
			NxFr::Path::HasExtension(Path, NxEn::AssetMetadata::Extension) ? Type::Asset : Type::File;

		GeneratePathInfo();
		GenerateImGuiText();
	}

	AssetsBrowserItem::~AssetsBrowserItem()
	{
	}

	void AssetsBrowserItem::GeneratePathInfo()
	{
		PathWithoutExtension = NxFr::Path::GetPathWithoutExtension(Path);
		Directory = NxFr::Path::GetParent(Path);
		Name = ItemType == Type::Directory ? NxFr::Path::GetDirectoryName(Path) : NxFr::Path::GetFileName(Path);
		Extension = NxFr::Path::GetExtension(Path);
		Depth = NxFr::Path::Split(Path).GetCount();
	}

	void AssetsBrowserItem::GenerateImGuiText()
	{
		ImGuiText = ItemType == Type::Directory ? "D" : ItemType == Type::Asset ? "A" : "F";
		ImGuiText += " ";
		ImGuiText += Name;
		ImGuiText += "##";
		ImGuiText += NxFr::StringUtility::ToString(Id);
	}

	bool AssetsBrowserItem::operator==(const AssetsBrowserItem& Other) const
	{
		return Id == Other.Id;
	}

	bool AssetsBrowserItem::operator!=(const AssetsBrowserItem& Other) const
	{
		return Id != Other.Id;
	}

	bool AssetsBrowserItem::operator<(const AssetsBrowserItem& Other) const
	{
		return Path < Other.Path;
	}

	bool AssetsBrowserItem::operator<=(const AssetsBrowserItem& Other) const
	{
		return Path <= Other.Path;
	}

	bool AssetsBrowserItem::operator>(const AssetsBrowserItem& Other) const
	{
		return Path > Other.Path;
	}

	bool AssetsBrowserItem::operator>=(const AssetsBrowserItem& Other) const
	{
		return Path >= Other.Path;
	}
}
