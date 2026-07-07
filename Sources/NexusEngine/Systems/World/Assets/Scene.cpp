#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Assets/Scene.h"

namespace NxEn
{
	NX_ASSET(Scene)

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

		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();

		Root = Factory->CreateGameObject(GetName(), NxFr::Handle<GameObject>());

		Root->ReferenceId = GetId();

		Root->Initialize();
		Root->SetEnabled(true);
	}

	void Scene::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<NxFr::StringView>::Property(Root->GetName(), "Root");
	}

	void Scene::OnClone(const Object& Other)
	{
		NX_ASSERT(false, Default, "Scene::Clone is not supported");
	}

	void Scene::OnSave(NxFr::StringView Path) const
	{
		NxFr::Yaml::SerializeFile(Root->Serialize(), Path);
	}

	void Scene::OnLoad(NxFr::StringView Path)
	{
		WorldObjectFactory* Factory = WorldObjectFactoryContext::GetFactory();
		YAML::Node Data = NxFr::Yaml::DeserializeFile(Path);

		Root = Factory->CreateGameObject("", NxFr::Handle<GameObject>(), Data["GameObject"]["Id"].as<NxFr::GUID>());
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
