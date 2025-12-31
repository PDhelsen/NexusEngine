#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Component;

	template<typename T>
	class ComponentsStorage;

	class ComponentsFactory
	{
	public:
		template<typename T>
		static ComponentsFactory* Register()
		{
			ComponentsFactory* Instance = new ComponentsStorage<T>();
			SetFactory(T::GetClassType(), Instance);
			return Instance;
		}

		NEXUS_ENGINE_API static ComponentsFactory* Create(NxFr::StringId Type);
		NEXUS_ENGINE_API static ComponentsFactory * GetFactory(NxFr::StringId Id);
		NEXUS_ENGINE_API static void SetFactory(NxFr::StringId Id, ComponentsFactory * Instance);

		NEXUS_ENGINE_API ComponentsFactory() = default;
		NEXUS_ENGINE_API ComponentsFactory(const ComponentsFactory& Other) = delete;
		NEXUS_ENGINE_API ComponentsFactory(ComponentsFactory&& Other) = delete;
		NEXUS_ENGINE_API virtual ~ComponentsFactory() = default;

		NEXUS_ENGINE_API virtual Component* Append() = 0;
		NEXUS_ENGINE_API virtual void Remove(uint64 Index) = 0;
		NEXUS_ENGINE_API virtual Component* Get(uint64 Index) = 0;

		NEXUS_ENGINE_API virtual void Reserve(uint64 Size) = 0;
		NEXUS_ENGINE_API virtual void Clear() = 0;

		virtual uint64 GetCount() const = 0;
		virtual uint64 GetCapacity() const = 0;

	protected:
		virtual ComponentsFactory* Clone() const = 0;
	};

	template<typename T>
	class ComponentsStorage : public ComponentsFactory
	{
	public:
		ComponentsStorage() = default;
		ComponentsStorage(const ComponentsStorage<T>& Other) = delete;
		ComponentsStorage(ComponentsStorage&& Other) = delete;
		virtual ~ComponentsStorage() { Clear(); }

		Component* Append() override { return &Instances.AppendConstruct(); }
		void Remove(uint64 Index) override { return Instances.RemoveSwap(Index); }
		Component* Get(uint64 Index) override { return &Instances[Index]; }

		void Reserve(uint64 Size) override { Instances.Reserve(Size); }
		void Clear() override { Instances.Clear(); }

		NxFr::List<T>::I Begin() const { return Instances.Begin(); }
		NxFr::List<T>::I End() const { return Instances.End(); }

		uint64 GetCount() const override { return Instances.GetCount(); }
		uint64 GetCapacity() const override { return Instances.GetCapacity(); }

	protected:
		virtual ComponentsFactory* Clone() const override { return new ComponentsStorage<T>(); };

	private:
		NxFr::List<T> Instances;
	};
}
