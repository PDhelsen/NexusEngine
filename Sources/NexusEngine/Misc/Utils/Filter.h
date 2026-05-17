#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	namespace Utils
	{
		class Filter
		{
		public:
			NEXUS_ENGINE_API Filter(NxFr::StringView Query);
			NEXUS_ENGINE_API ~Filter();

			NEXUS_ENGINE_API NxFr::Set<Object*> FilterObjects(const NxFr::Collection<Object*> Instances);
			NEXUS_ENGINE_API bool FilterObject(const Object& Instance);

			NEXUS_ENGINE_API bool FilterInstance(NxFr::StringView Substring, NxFr::GUID Id, NxFr::StringId Type);

		private:
			NxFr::String Query;
			NxFr::List<NxFr::StringView> Filters;

			NxFr::Array<NxFr::StringId> Types;
			NxFr::Array<NxFr::GUID> Ids;
			NxFr::Array<bool> Names;
			bool TypeAndString;
			bool All;
		};
	}
}
