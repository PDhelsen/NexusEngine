#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

namespace NxEd
{
	AssetsBrowserItem::AssetsBrowserItem()
		: Id(0), Label(""), Path(""), Type(0), Opened(false),
		Parent(0), Previous(0), Next(0), Child(0)
	{
	}

	AssetsBrowserItem::~AssetsBrowserItem()
	{
	}

	bool AssetsBrowserItem::Compare(const TreeItem& Other) const
	{
		return Path.C() < static_cast<const AssetsBrowserItem&>(Other).Path;
	}

	void AssetsBrowserItemDirectory::OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath)
	{
		NxFr::Directory(TargetPath).Create();
	}

	void AssetsBrowserItemDirectory::OnMove(NxFr::StringView CurrentPath, NxFr::StringView TargetPath)
	{
		NxFr::Directory(TargetPath).Create();
		NxFr::Directory(CurrentPath).Delete();
	}

	void AssetsBrowserItemDirectory::OnDuplicate(NxFr::StringView CurrentPath, NxFr::StringView TargetPath)
	{
		NxFr::Directory(TargetPath).Create();
	}

	void AssetsBrowserItemDirectory::OnDelete(NxFr::StringView CurrentPath)
	{
		NxFr::Directory(CurrentPath).Delete();
	}

	void AssetsBrowserItemContent::OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath)
	{
		NxFr::File(TargetPath).Create();
	}

	void AssetsBrowserItemContent::OnMove(NxFr::StringView CurrentPath, NxFr::StringView TargetPath)
	{
		NxFr::File(CurrentPath).Move(TargetPath);
	}

	void AssetsBrowserItemContent::OnDuplicate(NxFr::StringView CurrentPath, NxFr::StringView TargetPath)
	{
		NxFr::File(CurrentPath).Copy(TargetPath);
	}

	void AssetsBrowserItemContent::OnDelete(NxFr::StringView CurrentPath)
	{
		NxFr::File(CurrentPath).Delete();
	}

	void AssetsBrowserItemAsset::OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath)
	{
		NxFr::StringView AssetPath = NxFr::Path::GetPathWithoutExtension(TargetPath);
		NxFr::StringView Extension = NxFr::Path::GetExtension(TargetPath);

		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NxEn::Asset* Instance = Assets->Create(Type, AssetPath, Extension);
		Assets->Save(Instance->GetId());
	}

	void AssetsBrowserItemAsset::OnMove(NxFr::StringView CurrentPath, NxFr::StringView TargetPath)
	{
		NxFr::StringView AssetPath = NxFr::Path::GetPathWithoutExtension(TargetPath);

		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Assets->Move(GetId(), AssetPath);
	}

	void AssetsBrowserItemAsset::OnDuplicate(NxFr::StringView CurrentPath, NxFr::StringView TargetPath)
	{
		NxFr::StringView AssetPath = NxFr::Path::GetPathWithoutExtension(TargetPath);

		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Assets->Copy(GetId(), AssetPath);
	}

	void AssetsBrowserItemAsset::OnDelete(NxFr::StringView CurrentPath)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		Assets->Delete(GetId());
	}
}
