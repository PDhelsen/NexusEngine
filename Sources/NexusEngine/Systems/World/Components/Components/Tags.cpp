#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Components/Components/Tags.h"

namespace NxEn
{
	NX_COMPONENT_IMPLEMENTATION(Tags)

	Tags::Tags()
		: Items()
	{
	}

	Tags::~Tags()
	{
	}

	void Tags::Append(NxFr::StringView Tag)
	{
		Items.Append(Tag);
	}

	void Tags::Append(const NxFr::Collection<NxFr::StringView>& Tags)
	{
		for (const auto& It : Tags)
		{
			Append(It);
		}
	}

	void Tags::Remove(NxFr::StringView Tag)
	{
		Items.Remove(Tag);
	}

	void Tags::Remove(const NxFr::Collection<NxFr::StringView>& Tags)
	{
		for (const auto& It : Tags)
		{
			Remove(It);
		}
	}

	bool Tags::Contains(NxFr::StringView Tag)
	{
		return Items.TryGet(Tag);
	}

	bool Tags::Contains(const NxFr::Collection<NxFr::StringView>& Tags, bool MatchAll)
	{
		bool Result = MatchAll ? true : false;

		for (auto& It : Tags)
		{
			bool Match = Contains(It);
			Result = MatchAll ? Result & Match : Result | Match;

			if ((!MatchAll && Match) || (MatchAll && !Match))
			{
				break;
			}
		}

		return Result;
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
