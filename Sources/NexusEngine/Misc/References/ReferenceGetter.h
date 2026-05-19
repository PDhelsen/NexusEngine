#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	template<typename T>
	struct ReferenceGetter
	{
		using P = T;

		static P Get(T Instance)
		{
			return Instance;
		}
	};

	template<typename T>
	struct ReferenceGetter<NxFr::Handle<T>>
	{
		using P = T*;

		static P Get(NxFr::Handle<T> Instance)
		{
			return Instance ? Instance.GetRedirectedPointer() : nullptr;
		}
	};
}
