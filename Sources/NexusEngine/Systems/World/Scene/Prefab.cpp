#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Scene/Prefab.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"

namespace NxEn
{
	NEXUS_ASSET_IMPLEMENTATION(Prefab)

	NxFr::Handle<GameObject> Prefab::GetRoot() const
	{
		return Root;
	}

	void Prefab::SetRoot(NxFr::Handle<GameObject> Instance)
	{
		ObjectFactory& AssetsFactory = ObjectFactory::GetAssetsFactory();

		Root = AssetsFactory.DuplicateGameObject(Instance);
		Root->ReferenceId = 0;

		Instance->ReferenceId = GetId();

		SetDirty();
	}

	void Prefab::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		Node["Count"] = 1 + Root->GetChildCount(true);
		NxFr::Yaml::SerializeFile(Root->Save(), Content);
	}

	void Prefab::OnLoad(const YAML::Node& Node, NxFr::StringView Content)
	{
		ObjectFactory& AssetsFactory = ObjectFactory::GetAssetsFactory();

		AssetsFactory.Reserve(Node["Count"].as<uint64>());
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Content);

		Root = AssetsFactory.CreateGameObject("");
		Root->Load(Data);
		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Prefab::OnUnload()
	{
		ObjectFactory& AssetsFactory = ObjectFactory::GetAssetsFactory();

		Root->SetEnabled(false);
		Root->Shutdown();
		Root->Unload();

		AssetsFactory.DestroyGameObject(Root);
		Root = NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::GUID> Prefab::GetDependencies()
	{
		return Root->GetDependencies();
	}
}
