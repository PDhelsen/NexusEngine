#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Component/Components/Tags.h"

namespace NxEn
{
	NEXUS_COMPONENT_IMPLEMENTATION(Tags)

	Tags::Tags()
		: Items()
	{
	}

	Tags::~Tags()
	{
	}

	void Tags::OnGui(float TimeStep)
	{
		Component::OnGui(TimeStep);

		GUI::Drawer<NxFr::Set<NxFr::String>>::Field(Items, "Items");
	}

	void Tags::OnClone(const Object& Other)
	{
		Component::OnClone(Other);
		const Tags& Instance = static_cast<const Tags&>(Other);

		Items = Instance.Items;
	}

	void Tags::OnSave(YAML::Node& Node)
	{
		Node["Items"] = Items;
	}

	void Tags::OnLoad(const YAML::Node& Node)
	{
		Items = Node["Items"].as<NxFr::Set<NxFr::String>>();
	}

	void Tags::OnUnload()
	{
	}
}
