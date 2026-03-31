#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItem)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemDirectory)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemContent)
	NEXUS_OBJECT_IMPLEMENTATION(AssetsBrowserItemAsset)

	AssetsBrowserItem::AssetsBrowserItem()
		: TreeItem(), Id(0), Path(""), Type(0),
		Parent(nullptr), Previous(nullptr), Next(nullptr), Child(nullptr)
	{
	}

	AssetsBrowserItem::~AssetsBrowserItem()
	{
	}

	int8 AssetsBrowserItem::Compare(const TreeItem& Other) const
	{
		return NxFr::StringCApi::Compare(Path.C(), static_cast<const AssetsBrowserItem&>(Other).Path.C());
	}

	void AssetsBrowserItem::CacheImGuiText()
	{
		ImGuiText = GetPrefix() + " " + GetPrettyName() + "##" + NxFr::StringUtility::ToString(Id);
	}

	void AssetsBrowserItemDirectory::OnCreate(NxFr::StringView Target, NxFr::StringId Type)
	{
		NxFr::Directory(Target).Create();
	}

	void AssetsBrowserItemDirectory::OnMove(NxFr::StringView Item, NxFr::StringView Target)
	{
		NxFr::Directory(Target).Create();
		NxFr::Directory(Item).Delete();
	}

	void AssetsBrowserItemDirectory::OnDuplicate(NxFr::StringView Item, NxFr::StringView Target)
	{
		NxFr::Directory(Target).Create();
	}

	void AssetsBrowserItemDirectory::OnDelete(NxFr::StringView Item)
	{
		NxFr::Directory(Item).Delete();
	}

	void AssetsBrowserItemContent::OnCreate(NxFr::StringView Target, NxFr::StringId Type)
	{
		NxFr::File(Target).Create();
	}

	void AssetsBrowserItemContent::OnMove(NxFr::StringView Item, NxFr::StringView Target)
	{
		NxFr::File(Item).Move(Target);
	}

	void AssetsBrowserItemContent::OnDuplicate(NxFr::StringView Item, NxFr::StringView Target)
	{
		NxFr::File(Item).Copy(Target);
	}

	void AssetsBrowserItemContent::OnDelete(NxFr::StringView Item)
	{
		NxFr::File(Item).Delete();
	}

	void AssetsBrowserItemAsset::OnCreate(NxFr::StringView Target, NxFr::StringId Type)
	{
		NxFr::StringView AssetPath = NxFr::Path::GetPathWithoutExtension(Target);
		NxFr::StringView Extension = NxFr::Path::GetExtension(Target);

		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Create(Type, AssetPath, Extension);
	}

	void AssetsBrowserItemAsset::OnMove(NxFr::StringView Item, NxFr::StringView Target)
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Move(GetItemId(), NxFr::Path::GetPathWithoutExtension(Target));
	}

	void AssetsBrowserItemAsset::OnDuplicate(NxFr::StringView Item, NxFr::StringView Target)
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Copy(GetItemId(), NxFr::Path::GetPathWithoutExtension(Target));
	}

	void AssetsBrowserItemAsset::OnDelete(NxFr::StringView Item)
	{
		NxEn::Application::GetSystem<NxEn::AssetsSystem>()->Delete(GetItemId());
	}
}
