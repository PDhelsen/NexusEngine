#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserAction.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowserItem.h"
#include "NexusEditor/Systems/Assets/Browser/AssetsBrowser.h"

#include "NexusEditor/Systems/Assets/Importer/AssetImporter.h"
#include "NexusEditor/Systems/Object/Inspector/InspectorPanel.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerPanel.h"
#include "NexusEditor/Systems/Object/Viewer/ViewerContext.h"

#include "NexusEditor/Core/NexusEditorApplication.h"
#include "NexusEngine/Systems/GUI/Components/InputTextPopup.h"

namespace NxEd
{
	static bool IsInstantiable(NxEn::AssetsSystem* Assets, NxFr::GUID Id)
	{
		NxFr::StringId Type = Assets->GetMetadata(Id).GetType();
		return Type == NxEn::Scene::GetClassType() || Type == NxEn::Prefab::GetClassType();
	}

	void AssetsBrowserActionCreate::Execute(const NxFr::Array<NxEn::TreeItem*>&Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

			NxFr::StringView Name = NxFr::StringUtility::Split(Input, " ", 0);
			NxFr::StringView Type = NxFr::StringUtility::Split(Input, " ", 1);

			for (auto Item : Items)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);

				NxFr::StringView Directory = Instance->IsDirectory() ? Instance->GetTargetPath() : Instance->GetDirectory();
				NxFr::String Path = NxFr::Path::Combine(Directory, Name);

				Browser->Create(Path, Type);
			}
		});
	}

	void AssetsBrowserActionRename::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::InputTextPopup* Popup = NxEn::InputTextPopup::GetInstance();
		Popup->RegisterCallback([=](NxFr::StringView Input)
		{
			AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

			for (auto Item : Items)
			{
				AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);

				NxFr::String Path = Instance->GetTargetPath();
				NxFr::String Parent = Instance->GetDirectory();

				if (Instance->IsDirectory())
				{
					Path = NxFr::Path::Combine((NxFr::StringView)Parent, Input);
				}
				else
				{
					Path = NxFr::Path::Combine((NxFr::StringView)Parent, (Input + NxFr::Path::SeparatorExtension + Instance->GetExtension()));
				}

				Browser->Move(Instance->GetTargetPath(), Path);
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

		NX_ASSERT(Target->GetObjectType() == AssetsBrowserItemDirectory::GetClassType(), System, "Can only move AssetsBrowserItem to directory");

		for (uint64 Index = 1; Index < Items.GetCount(); ++Index)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Items[Index]);
			Browser->Move(Instance->GetTargetPath(), NxFr::Path::Combine(Target->GetTargetPath(), Instance->GetName()));
		}
	}

	void AssetsBrowserActionDelete::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		AssetsBrowser* Browser = NxEn::Application::GetInstance<NexusEditorApplication>()->GetAssetsBrowser();

		NxFr::Array<NxFr::String> Paths(Items.GetCount());
		for (uint64 Index = 0; Index < Paths.GetCount(); ++Index)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Items[Index]);
			Paths[Index] = Instance->GetTargetPath();
		}

		for (auto& Item : Paths)
		{
			if (Browser->Exist(Item))
			{
				Browser->Delete(Item);
			}
		}
	}

	void AssetsBrowserActionImport::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		for (auto& Item : Items)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
			if (Instance->GetObjectType() != AssetsBrowserItemContent::GetClassType())
			{
				continue;
			}

			AssetImporter::Run(Instance->GetTargetPath());
		}
	}

	void AssetsBrowserActionReimport::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		for (auto& Item : Items)
		{
			AssetsBrowserItem* Instance = static_cast<AssetsBrowserItem*>(Item);
			if (Instance->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			AssetImporter::Run(Instance->GetItemId());
		}
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
			if (IsInstantiable(Assets, Id))
			{
				NX_LOG(Warning, System, "Loading is not supported for this asset type. Use Instantiate instead");
				continue;
			}

			Assets->Load(Id);
		}
	}

	void AssetsBrowserActionReload::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();

		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			NxFr::GUID Id = Item->GetItemId();
			if (IsInstantiable(Assets, Id))
			{
				NX_LOG(Warning, System, "Loading is not supported for this asset type. Use Instantiate instead");
				continue;
			}

			Assets->Reload(Item->GetItemId());
		}
	}

	void AssetsBrowserActionInstantiate::Execute(const NxFr::Array<NxEn::TreeItem*>& Items)
	{
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NxEn::AssetsSystem* Assets = NxEn::Application::GetSystem<NxEn::AssetsSystem>();
		NexusEditorApplication* Editor = NxEn::Application::GetInstance<NexusEditorApplication>();
		NxFr::GUID WorldId = NxEn::WorldSystem::WorldId.GetId();

		for (auto& Item : Items)
		{
			if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
			{
				continue;
			}

			NxFr::GUID Id = Item->GetItemId();
			NxFr::StringId Type = Assets->GetMetadata(Id).GetType();

			if (Type == NxEn::Scene::GetClassType())
			{
				Worlds->LoadScene(Id, WorldId);
			}
			else if (Type == NxEn::Prefab::GetClassType())
			{
				NxEn::Prefab* Instance = Worlds->LoadPrefab(Id);
				Worlds->InstantiatePrefab(Instance, NxFr::Handle<NxEn::GameObject>(), WorldId);
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
		NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
		NexusEditorApplication* Editor = NxEn::Application::GetInstance<NexusEditorApplication>();

		AssetsBrowserItem* Item = static_cast<AssetsBrowserItem*>(Items[0]);
		if (Item->GetObjectType() != AssetsBrowserItemAsset::GetClassType())
		{
			return;
		}

		NxEn::Object* Target = nullptr;
		NxFr::GUID Id = Item->GetItemId();
		NxFr::StringId Type = Assets->GetMetadata(Id).GetType();

		if (Type == NxEn::Scene::GetClassType())
		{
			Target = nullptr;
		}
		else if (Type == NxEn::Prefab::GetClassType())
		{
			Target = Worlds->LoadPrefab(Id);
		}
		else
		{
			Target = Assets->Load(Id);
		}

		StageManager* Manager = Editor->GetStageManager();
		Stage* StageView = Manager->GetStage(Target);
		if (!StageView)
		{
			StageView = Manager->CreateStage(Target);
		}
		Manager->ShowStage(Target);
	}
}
