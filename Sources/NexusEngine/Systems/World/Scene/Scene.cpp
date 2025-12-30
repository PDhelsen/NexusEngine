#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Scene/Scene.h"
#include "NexusEngine/Systems/World/ObjectFactory.h"

namespace NxEn
{
	NEXUS_ASSET_IMPLEMENTATION(Scene)

	NxFr::Handle<GameObject> Scene::GetRoot() const
	{
		return Root;
	}

	void Scene::OnInitialize()
	{
		if (Root)
		{
			return;
		}

		AssetMetadata& Metadata = Application::GetSystem<AssetsSystem>()->GetMetadata(GetId());
		ObjectFactory& Factory = FactoryContext::GetFactory();

		Root = Factory.CreateGameObject(Metadata.GetName(), NxFr::Handle<GameObject>());

		Root->ReferenceId = GetId();

		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Scene::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Yaml::SerializeFile(Root->Save(), Content);
	}

	void Scene::OnLoad(const YAML::Node& Node, NxFr::StringView Content)
	{
		ObjectFactory& Factory = FactoryContext::GetFactory();
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Content);

		Root = Factory.CreateGameObject("", NxFr::Handle<GameObject>(), GameObject::ReadIdFromYaml(Data));
		Root->Load(Data);
		Root->PatchReferences();
		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Scene::OnUnload()
	{
		ObjectFactory& Factory = FactoryContext::GetFactory();

		Root->SetEnabled(false);
		Root->Shutdown();
		Root->Unload();

		Factory.DestroyGameObject(Root);
		Root = NxFr::Handle<GameObject>();
	}

}
