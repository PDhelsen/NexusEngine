#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Systems/World/Component/ComponentsFactory.h"

namespace NxEn
{
	static NxFr::Dictionary<NxFr::StringId, ComponentsFactory*>& GetFactories()
	{
		static NxFr::Dictionary<NxFr::StringId, ComponentsFactory*> Factories;
		return Factories;
	}

	ComponentsFactory* ComponentsFactory::GetFactory(NxFr::StringId Id)
	{
		return GetFactories()[Id];
	}

	void ComponentsFactory::SetFactory(NxFr::StringId Id, ComponentsFactory* Instance)
	{
		GetFactories().AppendOrAssign(Id, Instance);
	}

	Component* ComponentsFactory::Append()
	{
		return Instance->Append();
	}

	void ComponentsFactory::Remove(uint64 Index)
	{
		Instance->Remove(Index);
	}

	Component* ComponentsFactory::Get(uint64 Index)
	{
		return Instance->Get(Index);
	}

	void ComponentsFactory::Reserve(uint64 Index)
	{
		Instance->Reserve(Index);
	}

	void ComponentsFactory::Clear()
	{
		Instance->Clear();
	}

	NxFr::Iterator::IteratorBlock<Component> ComponentsFactory::Begin()
	{
		return Instance->Begin();
	}

	NxFr::Iterator::IteratorBlock<Component> ComponentsFactory::End()
	{
		return Instance->End();
	}

	uint64 ComponentsFactory::GetCount() const
	{
		return Instance->GetCount();
	}

	uint64 ComponentsFactory::GetCapacity() const
	{
		return Instance->GetCapacity();
	}

	ComponentsFactory::ComponentsFactory(Factory* Instance)
		: Instance(Instance)
	{
	}

	ComponentsFactory::~ComponentsFactory()
	{
		Instance->Clear();
		delete Instance;
	}
}
