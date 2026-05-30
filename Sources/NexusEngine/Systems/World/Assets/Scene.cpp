#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Assets/Scene.h"

namespace NxEn
{
	NX_ASSET_IMPLEMENTATION(Scene)

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
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();

		Root = Factory->CreateGameObject(Metadata.GetName(), NxFr::Handle<GameObject>());

		Root->ReferenceId = GetId();

		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Scene::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<NxFr::StringView>::Property(Root->GetName(), "Root");
	}

	void Scene::OnSave(YAML::Node& Node, NxFr::StringView Content)
	{
		NxFr::Yaml::SerializeFile(Root->Serialize(), Content);
	}

	void Scene::OnLoad(const YAML::Node& Node, NxFr::StringView Content)
	{
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Content);

		Root = Factory->CreateGameObject("", NxFr::Handle<GameObject>(), GameObject::ReadIdFromYaml(Data));
		Root->Deserialize(Data);
		Root->PatchReferences();
		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Scene::OnUnload()
	{
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();

		Root->SetEnabled(false);
		Root->Shutdown();
		Root->Unload();

		Factory->DestroyGameObject(Root);
		Root = NxFr::Handle<GameObject>();
	}

}
