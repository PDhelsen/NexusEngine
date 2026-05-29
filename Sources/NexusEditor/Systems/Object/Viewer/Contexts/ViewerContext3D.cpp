#include "NexusEditor/Systems/Object/Viewer/Contexts/ViewerContext3D.h"

namespace NxEd
{
	static ViewerFactory::Factory* FactoryWorld = ViewerFactory::Register<NxEn::World, ViewerContext3D>();
	static ViewerFactory::Factory* FactoryPrefab = ViewerFactory::Register<NxEn::Prefab, ViewerContext3D>();

	ViewerContext3D::ViewerContext3D()
		: Mode(), World(nullptr), Target()
	{

	}

	ViewerContext3D::~ViewerContext3D()
	{
		Clear();
	}

	void ViewerContext3D::Clear()
	{
		if (Mode == ViewContextMode::Prefab)
		{
			NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
			Worlds->DestroyWorld(World->GetId());
		}
	}

	void ViewerContext3D::SetupMenu(NxEn::GUI::Menu& Menu)
	{
	}

	void ViewerContext3D::SetupTarget(NxEn::Object* Instance)
	{
		if (Instance->GetObjectType() == NxEn::World::GetClassType())
		{
			Mode = ViewContextMode::World;

			World = static_cast<NxEn::World*>(Instance);
			Target = World->GetRootGameObject();
		}
		else if (Instance->GetObjectType() == NxEn::Prefab::GetClassType())
		{
			Mode = ViewContextMode::Prefab;

			NxEn::WorldSystem* Worlds = NxEn::Application::GetSystem<NxEn::WorldSystem>();
			NxEn::Prefab* Prefab = static_cast<NxEn::Prefab*>(Instance);

			World = Worlds->CreateWorld(Prefab->GetId());
			Target = Worlds->InstantiatePrefab(Prefab, World->GetRootGameObject(), World->GetId());
			Target->SetName(Prefab->GetName());
		}
		else
		{
			Mode = ViewContextMode::Invalid;

			NX_LOG(Error, System, "Unsupported type for ViewerContext3D")
		}
	}

	void ViewerContext3D::OnDraw()
	{
		NxEn::GUI::Drawer<NxFr::StringView>::Property(Target->GetName());
	}
}
