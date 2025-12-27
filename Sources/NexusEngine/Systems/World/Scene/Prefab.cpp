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
		FactoryContext Context(FactoryContext::GetAssetsFactory());
		ObjectFactory& Factory = FactoryContext::GetFactory();

		Root = Factory.DuplicateGameObject(Instance, NxFr::Handle<GameObject>(), true);

		Root->ReferenceId = GetId();
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
		FactoryContext Context(FactoryContext::GetAssetsFactory());
		ObjectFactory& Factory = FactoryContext::GetFactory();

		Factory.Reserve(Node["Count"].as<uint64>());
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Content);

		Root = Factory.CreateGameObject("", NxFr::Handle<GameObject>(), GameObject::ReadIdFromYaml(Data));

		Root->Load(Data);
		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Prefab::OnUnload()
	{
		FactoryContext Context(FactoryContext::GetAssetsFactory());
		ObjectFactory& Factory = FactoryContext::GetFactory();

		Root->SetEnabled(false);
		Root->Shutdown();
		Root->Unload();

		Factory.DestroyGameObject(Root);
		Root = NxFr::Handle<GameObject>();
	}

	NxFr::Array<NxFr::GUID> Prefab::GetDependencies()
	{
		NxFr::Set<NxFr::GUID> Dependencies;
		Root->GatherDependencies(Dependencies);
		return NxFr::ContainersUtils::ToArray<NxFr::GUID>(Dependencies);
	}
}
