#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	class Asset;

	class AssetsFactory
	{
	public:
		struct Factory
		{
			virtual Asset* Create() const = 0;
		};

		template<typename T>
		static Factory* Register()
		{
			Factory* Instance = new FactoryTyped<T>();
			SetFactory(T::GetClassType(), Instance);
			return Instance;
		}

		static Asset* Create(NxFr::StringId Id)
		{
			return GetFactory(Id)->Create();
		}

	private:
		template<typename T>
		struct FactoryTyped : public Factory
		{
			Asset* Create() const override { return new T(); };
		};

		NX_ENGINE_API static Factory* GetFactory(NxFr::StringId Id);
		NX_ENGINE_API static void SetFactory(NxFr::StringId Id, Factory* Instance);
	};
}
