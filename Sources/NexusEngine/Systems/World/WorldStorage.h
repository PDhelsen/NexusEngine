#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class WorldStorage
	{
		friend class WorldManager;

	public:
		static NxFr::Factory<WorldStorage>& GetFactory();

		WorldStorage() = default;
		virtual ~WorldStorage() = default;

		virtual bool IsEmpty() const = 0;
		virtual uint64 GetCount() const = 0;

	protected:
		virtual Object& Append() = 0;
		virtual void Remove(uint64 Index) = 0;
		virtual Object& Get(uint64 Index) = 0;
		virtual void Reserve(uint64 Size) = 0;

		virtual uint64 GetCapacity() const = 0;
	};

	template<typename T>
	class WorldStorageTyped : public WorldStorage
	{
	public:
		WorldStorageTyped() : Instances() {}
		virtual ~WorldStorageTyped() {}

		bool IsEmpty() const override { return Instances.IsEmpty(); };
		uint64 GetCount() const override { return Instances.GetCount(); }

	protected:
		Object& Append() override { return Instances.AppendConstruct(); }
		void Remove(uint64 Index) override { Instances.RemoveSwap(Index); }
		Object& Get(uint64 Index) override { return Instances.Get(Index); };
		void Reserve(uint64 Size) override { Instances.Reserve(Size); }

		uint64 GetCapacity() const override { return Instances.GetCapacity(); }

	private:
		NxFr::List<T> Instances;
	};
}
