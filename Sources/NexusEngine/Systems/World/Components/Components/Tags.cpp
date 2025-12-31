#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Components/Components/Tags.h"

#include "NexusEngine/Systems/World/Factory/WorldObjectStorage.h"

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
		Component::OnSave(Node);

		Node["Items"] = Items;
	}

	void Tags::OnLoad(const YAML::Node& Node)
	{
		Component::OnLoad(Node);

		Items = Node["Items"].as<NxFr::Set<NxFr::String>>();
	}
}
