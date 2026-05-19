#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class ViewerContext;

	class NX_EDITOR_API ViewerFactory
	{
	public:
		struct Factory
		{
			virtual ViewerContext* Create() const = 0;
		};

		template<typename T, typename F>
		static Factory* Register()
		{
			Factory* Instance = new FactoryTyped<F>();
			SetFactory(T::GetClassType(), Instance);
			return Instance;
		}

		static ViewerContext* Create(NxFr::StringId Id)
		{
			Factory* Instance = GetFactory(Id);
			return Instance ? Instance->Create() : nullptr;
		}

	private:
		template<typename F>
		struct FactoryTyped : public Factory
		{
			ViewerContext* Create() const override { return new F(); };
		};

		static Factory* GetFactory(NxFr::StringId Id);
		static void SetFactory(NxFr::StringId Id, Factory* Instance);
	};
}
