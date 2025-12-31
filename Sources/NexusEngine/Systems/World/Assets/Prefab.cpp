#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Assets/Prefab.h"

namespace NxEn
{
	NEXUS_ASSET_IMPLEMENTATION(Prefab)

	NxFr::Handle<GameObject> Prefab::GetRoot() const
	{
		return Root;
	}

	void Prefab::SetRoot(NxFr::Handle<GameObject> Instance)
	{
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();

		Root = Factory->DuplicateGameObject(Instance, NxFr::Handle<GameObject>(), true);

		Root->ReferenceId = GetId();
		Instance->ReferenceId = GetId();

		Root->PatchReferences();
		SetDirty();
	}

	void Prefab::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Yaml::SerializeFile(Root->Save(), Content);
	}

	void Prefab::OnLoad(const YAML::Node& Node, NxFr::StringView Content)
	{
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Content);

		Root = Factory->CreateGameObject("", NxFr::Handle<GameObject>(), GameObject::ReadIdFromYaml(Data));
		Root->Load(Data);
		Root->PatchReferences();
		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Prefab::OnUnload()
	{
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();

		Root->SetEnabled(false);
		Root->Shutdown();
		Root->Unload();

		Factory->DestroyGameObject(Root);
		Root = NxFr::Handle<GameObject>();
	}
}
