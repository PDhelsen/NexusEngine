#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"
#include "NexusEditor/Core/NexusEditorApplication.h"
#include "NexusEngine/Misc/GUI/InputTextPopup.h"

namespace NxEd
{
	void AssetsBrowserActionCreate::Execute(const NxFr::Array<NxEn::TreeItem*>&Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::Application::GetSystem<NxEn::GUISystem>()->TryReuseElement<NxEn::InputTextPopup>();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

			NxFr::StringView Name = NxFr::StringUtility::Split(Input, " ", 0);
			NxFr::StringView Type = NxFr::StringUtility::Split(Input, " ", 1);

			for (auto Item : Items)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
				if (Instance->GetObjectType() != AssetsBrowserItemDirectory::GetClassType())
				{
					NX_LOG(Error, System, "Can only create AssetsBrowserItem in directory");
					continue;
				}

				NxFr::String Path = NxFr::Path::Combine(Instance->GetTargetPath(), Name);
				Browser->Create(Type, Path);
			}
		});
	}

	void AssetsBrowserActionRename::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::Application::GetSystem<NxEn::GUISystem>()->TryReuseElement<NxEn::InputTextPopup>();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

			for (auto Item : Items)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
				Browser->Move(Instance->GetTargetPath(), NxFr::Path::ChangeName(Instance->GetTargetPath(), Input));
			}
		});
	}

	void AssetsBrowserActionDuplicate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

		for (auto& Item : Items)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
			Browser->Duplicate(Instance->GetTargetPath(), Instance->GetTargetPath());
		}
	}

	void AssetsBrowserActionMove::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		if (Items.GetCount() <= 1)
		{
			return;
		}

		AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();
		AssetsBrowserItem* Target = static_cast<AssetsBrowserItem*>(Items[0]);
		if (Target->GetObjectType() != AssetsBrowserItemDirectory::GetClassType())
		{
			NX_LOG(Error, System, "Can only move AssetsBrowserItem in directory");
			return;
		}

		for (uint64 Index = 1; Index < Items.GetCount(); ++Index)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Items[Index]);
			Browser->Move(Instance->GetTargetPath(), NxFr::Path::ChangeFolder(Instance->GetTargetPath(), Target->GetTargetPath()));
		}
	}

	void AssetsBrowserActionDelete::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

		NxFr::Array<NxFr::String> Paths(Items.GetCount());
		for (uint64 Index = 0; Index < Paths.GetCount(); ++Index)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Items[Index]);
			Browser->Delete(Instance->GetTargetPath());
		}
	}

	void AssetsBrowserActionImport::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

		for (auto& Item : Items)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
			if (Instance->GetObjectType() == AssetsBrowserItemContent::GetClassType())
			{
				AssetImporter::Run(Instance->GetTargetPath());
			}
			else if (Instance->GetObjectType() == AssetsBrowserItemAsset::GetClassType())
			{
				AssetImporter::Run(Instance->GetItemId());
			}
		}

		Browser->Refresh();
	}

	void AssetsBrowserActionLoad::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();

		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			NxFr::GUID Id = Item->GetItemId();
			Assets->Reload(Id);
		}
	}

	void AssetsBrowserActionInstantiate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();

		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			NxFr::GUID Id = Item->GetItemId();
			NxFr::StringId Type = Assets->GetMetadata(Id)->GetType();

			if (Type == NxEn::Prefab::GetClassType())
			{
				NxEn::Prefab* Instance = Assets->Load<NxEn::Prefab>(Id);
				Worlds->InstantiateGameObject(Instance->GetRoot());
			}
			else if (Type == NxEn::Scene::GetClassType())
			{
				NxEn::Scene* Instance = Assets->Load<NxEn::Scene>(Id);
				Worlds->InstantiateScene(Instance);
			}
			else
			{
				NX_LOG(Warning, System, "Instantiate is not supported for this asset type. Use Load instead");
			}
		}
	}

	void AssetsBrowserActionView::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NexusEditorApplication* Editor = NxEn::Application::GetInstance<NexusEditorApplication>();

		AssetsBrowserItem* Item = static_cast<AssetsBrowserItem*>(Items[0]);
		if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
		{
			return;
		}

		NxFr::GUID Id = Item->GetItemId();
		NxEn::Object* Target = Assets->Load(Id);

		StageManager* Manager = Editor->GetStageManager();
		Stage* StageView = Manager->GetStage(Target);
		if (!StageView)
		{
			StageView = Manager->CreateStage(Target);
		}
		Manager->ShowStage(Target);
	}
}
