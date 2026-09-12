#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Scene.h"

#include "NexusEngine/Systems/Resources/Resources/Text.h"

namespace NxEn
{
	NX_ASSET(Scene)

	void Scene::OnInitialize()
	{
		if (Root)
		{
			return;
		}

		Text* TemplateScene = Application::GetSystem<ResourcesSystem>()->Load<Text>("TemplateScene.scene");
		NxFr::String NewScene = NxFr::StringUtility::FormatTo(TemplateScene->GetText(), GetName().C(), GetId());

		NxFr::Yaml::Node Data = NxFr::Yaml::Deserialize(NewScene);
		SetRoot(Application::GetSystem<WorldSystem>()->DeserializeGameObject(Data));
	}

	void Scene::OnDraw()
	{
		Asset::OnDraw();

		GUI::Drawer<NxFr::StringView>::Property(Root ? Root->GetName() : "", "Root");
	}

	void Scene::OnClone(const Object& Other)
	{
		NX_ASSERT(false, Default, "Scene::Clone is not supported");
	}

	void Scene::OnSave(NxFr::StringView Path) const
	{
		NxFr::Yaml::Node Data = Application::GetSystem<WorldSystem>()->SerializeGameObject(Root);
		NxFr::Yaml::SerializeAndSave(Data, Path);
	}

	void Scene::OnLoad(NxFr::StringView Path)
	{
		NxFr::Yaml::Node Data = NxFr::Yaml::LoadAndDeserialize(Path);
		SetRoot(Application::GetSystem<WorldSystem>()->DeserializeGameObject(Data));
	}

	void Scene::OnUnload()
	{
		if (!Root)
		{
			return;
		}

		Application::GetSystem<WorldSystem>()->UnloadGameObject(Root);

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Release(Dependency);
		}

		Root = NxFr::Handle<GameObject>();
	}

	void Scene::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const
	{
		if (!Root)
		{
			return;
		}

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = Root->Object::GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			if (Assets->IsTracked(Dependency) && Dependency != GetId())
			{
				Ids.Append(Dependency);
			}
		}
	}

	void Scene::SetRoot(NxFr::Handle<GameObject> Instance)
	{
		Unload();

		Root = Instance;
		SetDirty();

		AssetsSystem* Assets = Application::GetSystem<AssetsSystem>();
		NxFr::Array<NxFr::GUID> Dependencies = GetDependencies();
		for (auto& Dependency : Dependencies)
		{
			Assets->Acquire(Dependency);
		}
	}
}
