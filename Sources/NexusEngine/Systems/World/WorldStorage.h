#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/World/Misc/Iterator.h"

#define NX_WORLD_OBJECT(Type) static NxFr::Factory<NxEn::WorldStorage>::Creator* Factory##Type = NxEn::WorldStorage::GetFactory().Register<NxEn::WorldStorageOf<Type>>(Type::GetClassType());

namespace NxEn
{
	class WorldStorage
	{
		friend class WorldManager;
		friend struct Iterator::WorldObject;

	public:
		static NxFr::Factory<WorldStorage>& GetFactory();

		WorldStorage() = default;
		virtual ~WorldStorage() = default;

	protected:
		virtual Object& Append() = 0;
		virtual void Remove(uint64 Index) = 0;
		virtual Object& Get(uint64 Index) = 0;
		virtual const Object& Get(uint64 Index) const = 0;
		virtual void Reserve(uint64 Size) = 0;

		Iterator::WorldObject Begin() { return Iterator::WorldObject(this, 0); }
		Iterator::WorldObject End() { return Iterator::WorldObject(this, GetCount()); }

		virtual bool IsEmpty() const = 0;
		virtual uint64 GetCount() const = 0;
		virtual uint64 GetCapacity() const = 0;
	};

	template<typename T>
	class WorldStorageOf : public WorldStorage
	{
	public:
		WorldStorageOf() : Instances() {}
		virtual ~WorldStorageOf() {}

	protected:
		T& Append() override { return Instances.AppendConstruct(); }
		void Remove(uint64 Index) override { Instances.RemoveSwap(Index); }
		T& Get(uint64 Index) override { return Instances.Get(Index); }
		const T& Get(uint64 Index) const override { return Instances.Get(Index); }
		void Reserve(uint64 Size) override { Instances.Reserve(Size); }

		bool IsEmpty() const override { return Instances.IsEmpty(); };
		uint64 GetCount() const override { return Instances.GetCount(); }
		uint64 GetCapacity() const override { return Instances.GetCapacity(); }

	private:
		NxFr::List<T> Instances;
	};
}
