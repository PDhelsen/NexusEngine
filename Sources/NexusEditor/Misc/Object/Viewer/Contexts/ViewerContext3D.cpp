#include "NexusEditor/Misc/Object/Viewer/Contexts/ViewerContext3D.h"

namespace NxEd
{
	NxFr::Factory<ViewerContext>::Creator* FactoryWorld = ViewerPanel::GetFactory().Register<ViewerContext3D>(NxEn::World::GetClassType());

	void ViewerContext3D::Clear()
	{

	}

	void ViewerContext3D::Setup(NxEn::Object* Instance)
	{
		NX_ASSERT_RETURN(Instance->GetObjectType() == NxEn::World::GetClassType(), , Default, "Viewer 3D expect a world");

		Target = Instance;
	}

	void ViewerContext3D::Draw()
	{
		NxEn::GUI::Transform Transform = NxEn::GUI::Transform(-NxFr::Vector2f::One, NxFr::Vector2f::Zero, -1.0f);
		NxEn::GUI::Drawer<NxFr::StringView>::Property(Target->GetName(), "", Transform);
	}
}
