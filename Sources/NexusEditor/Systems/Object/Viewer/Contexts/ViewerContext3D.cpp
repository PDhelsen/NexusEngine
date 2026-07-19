#include "NexusEditor/Systems/Object/Viewer/Contexts/ViewerContext3D.h"

namespace NxEd
{
	static ViewerFactory::Factory* FactoryWorld = ViewerFactory::Register<NxEn::World, ViewerContext3D>();

	ViewerContext3D::ViewerContext3D()
		: Target()
	{

	}

	ViewerContext3D::~ViewerContext3D()
	{
		Clear();
	}

	void ViewerContext3D::Clear()
	{

	}

	void ViewerContext3D::SetupMenu(NxEn::GUI::Menu& Menu)
	{
	}

	void ViewerContext3D::SetupTarget(NxEn::Object* Instance)
	{
		NX_ASSERT(Instance->GetObjectType() == NxEn::World::GetClassType(), Default, "Viewer 3D expect a world");

		Target = Instance;
	}

	void ViewerContext3D::OnDraw()
	{
		NxEn::GUI::Drawer<NxFr::StringView>::Property(Target->GetName());
	}
}
