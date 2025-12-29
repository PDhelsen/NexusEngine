#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Component;

	class ComponentsFactory
	{
		struct Factory
		{
			virtual Factory* Clone() = 0;

			virtual Component* Append() = 0;
			virtual void Remove(uint64 Index) = 0;
			virtual Component* Get(uint64 Index) = 0;

			virtual void Reserve(uint64 Size) = 0;
			virtual void Clear() = 0;

			virtual NxFr::Iterator::IteratorBlock<Component> Begin() const = 0;
			virtual NxFr::Iterator::IteratorBlock<Component> End() const = 0;

			virtual uint64 GetCount() const = 0;
			virtual uint64 GetCapacity() const = 0;
		};

		template<typename T>
		struct FactoryTyped : public Factory
		{
			Factory* Clone() override { return new FactoryTyped<T>(); };

			Component* Append() override { return &Instances.AppendConstruct(); }
			void Remove(uint64 Index) override { return Instances.RemoveSwap(Index); }
			Component* Get(uint64 Index) override { return &Instances[Index]; }

			void Reserve(uint64 Size) override { Instances.Reserve(Size); }
			void Clear() override { Instances.Clear(); }

			NxFr::Iterator::IteratorBlock<Component> Begin() const override { return Instances.Begin(); }
			NxFr::Iterator::IteratorBlock<Component> End() const override { return Instances.End(); }

			uint64 GetCount() const override { return Instances.GetCount(); }
			uint64 GetCapacity() const override { return Instances.GetCapacity(); }

			NxFr::List<T> Instances;
		};

	public:
		template<typename T>
		static ComponentsFactory* Register()
		{
			ComponentsFactory* Instance = new ComponentsFactory(new FactoryTyped<T>());
			SetFactory(T::GetClassType(), Instance);
			return Instance;
		}

		static ComponentsFactory* Create(NxFr::StringId Type)
		{
			ComponentsFactory* Template = GetFactory(Type);
			return new ComponentsFactory(Template->Instance->Clone());
		}

		NEXUS_ENGINE_API static ComponentsFactory* GetFactory(NxFr::StringId Id);
		NEXUS_ENGINE_API static void SetFactory(NxFr::StringId Id, ComponentsFactory* Instance);

		NEXUS_ENGINE_API ComponentsFactory(Factory* Instance);
		NEXUS_ENGINE_API ~ComponentsFactory();

		NEXUS_ENGINE_API Component* Append();
		NEXUS_ENGINE_API void Remove(uint64 Index);
		NEXUS_ENGINE_API Component* Get(uint64 Index);

		NEXUS_ENGINE_API void Reserve(uint64 Size);
		NEXUS_ENGINE_API void Clear();

		NEXUS_ENGINE_API NxFr::Iterator::IteratorBlock<Component> Begin();
		NEXUS_ENGINE_API NxFr::Iterator::IteratorBlock<Component> End();

		NEXUS_ENGINE_API uint64 GetCount() const;
		NEXUS_ENGINE_API uint64 GetCapacity() const;

	private:
		Factory* Instance;
	};
}
