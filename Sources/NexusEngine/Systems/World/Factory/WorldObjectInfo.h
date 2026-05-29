#pragma once

#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	struct WorldObjectInfo
	{
		NxFr::Handle<Object> Handle;
		uint64 Index;
	};
}
