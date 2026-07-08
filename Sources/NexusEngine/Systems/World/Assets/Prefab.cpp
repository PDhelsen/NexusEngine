#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Assets/Prefab.h"

namespace NxEn
{
	NX_ASSET(Prefab)

	NxFr::Handle<GameObject> Prefab::GetRoot() const
	{
		return Root;
	}

	void Prefab::SetRoot(NxFr::Handle<GameObject> Instance)
	{
		WorldObjectFactory* Factory = WorldObjectFactory::GetContexts().TryGet();

		Root = Factory->DuplicateGameObject(Instance, NxFr::Handle<GameObject>(), true);

		Root->ReferenceId = GetId();
		Instance->ReferenceId = GetId();

		Root->PatchReferences();
		SetDirty();
	}

	void Prefab::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<NxFr::StringView>::Property(Root->GetName(), "Root");
	}

	void Prefab::OnClone(const Object& Other)
	{
		NX_ASSERT(false, Default, "Prefab::Clone is not supported. Please use WorldSystem::InstantiatePrefab instead");
	}

	void Prefab::OnSave(NxFr::StringView Path) const
	{
		NxFr::Yaml::SerializeFile(Root->Serialize(), Path);
	}

	void Prefab::OnLoad(NxFr::StringView Path)
	{
		WorldObjectFactory* Factory = WorldObjectFactory::GetContexts().TryGet();
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Path);

		Root = Factory->CreateGameObject("", NxFr::Handle<GameObject>(), Data["GameObject"]["Id"].as<NxFr::GUID>());
		Root->Deserialize(Data);
		Root->PatchReferences();
		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Prefab::OnUnload()
	{
		WorldObjectFactory* Factory = WorldObjectFactory::GetContexts().TryGet();

		Root->SetEnabled(false);
		Root->Shutdown();
		Root->Unload();

		Factory->DestroyGameObject(Root);
		Root = NxFr::Handle<GameObject>();
	}
}
