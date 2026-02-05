#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

namespace NxEn
{
	template<typename T>
	struct ReferenceInstance
	{
		static T Get(T Ptr)
		{
			return Ptr;
		}
	};

	template<typename T>
	struct ReferenceInstance<NxFr::Handle<T>>
	{
		static T* Get(NxFr::Handle<T> Handle)
		{
			return Handle ? Handle.GetRedirectedPointer() : nullptr;
		}
	};
}
