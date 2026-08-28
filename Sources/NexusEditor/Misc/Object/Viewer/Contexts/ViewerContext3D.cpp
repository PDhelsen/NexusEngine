#include "NexusEditor/Misc/Object/Viewer/Contexts/ViewerContext3D.h"

#include "NexusEngine/Systems/World/World.h"

namespace NxEd
{
	NxFr::Factory<ViewerContext>::Creator* FactoryWorld = ViewerPanel::GetFactory().Register<ViewerContext3D>(NxEn::World::GetClassType());

	void ViewerContext3D::Clear()
	{

	}

	void ViewerContext3D::Setup(NxEn::ObjectInstance<NxEn::Object> Target)
	{
		NX_ASSERT_RETURN(Target->GetObjectType() == NxEn::World::GetClassType(), , Default, "Viewer 3D expect a world");

		Instance = static_cast<NxEn::World*>(Target.Get());
	}

	void ViewerContext3D::Draw()
	{
		NxEn::GUI::Transform Transform = NxEn::GUI::Transform(-NxFr::Vector2f::One, NxFr::Vector2f::Zero, -1.0f);
		NxEn::GUI::Drawer<NxFr::StringView>::Property(Instance->GetName(), "", Transform);
	}
}
