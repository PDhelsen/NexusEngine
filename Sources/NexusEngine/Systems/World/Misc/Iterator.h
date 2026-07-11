#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	class WorldStorage;
	class GameObject;

	namespace Iterator
	{
		struct NX_ENGINE_API WorldObject : public NxFr::Iterator::Iterator<Object, WorldObject>
		{
		public:
			WorldObject(WorldStorage* Storage, uint64 Index = 0);

			bool Equals(const WorldObject& Other) const;
			void Increment();
			void Decrement();
			Object& Get();
			const Object& Get() const;
			uint64 Id() const;

		private:
			WorldStorage* Storage;
			uint64 Index;
		};

		template<typename T>
		struct WorldObjectOf : public NxFr::Iterator::Iterator<T, WorldObjectOf<T>>
		{
		public:
			WorldObjectOf(WorldObject It)
				: It(It)
			{

			}

			bool Equals(const WorldObjectOf<T>& Other) const
			{
				return It.Equals(Other.It);
			}

			void Increment()
			{
				It.Increment();
			}

			void Decrement()
			{
				It.Decrement();
			}

			T& Get()
			{
				return static_cast<T&>(It.Get());
			}

			const T& Get() const
			{
				return static_cast<const T&>(It.Get());
			}

			uint64 Id() const
			{
				return It.Id();
			}

		private:
			WorldObject It;
		};

		struct NX_ENGINE_API WorldHierarchy : public NxFr::Iterator::Iterator<GameObject, WorldHierarchy>
		{
		public:
			WorldHierarchy(NxFr::Handle<GameObject> Instance);

			bool Equals(const WorldHierarchy& Other) const;
			void Increment();
			void Decrement();
			GameObject& Get();
			const GameObject& Get() const;
			uint64 Id() const;

		private:
			NxFr::Handle<GameObject> Instance;
		};
	}
}
