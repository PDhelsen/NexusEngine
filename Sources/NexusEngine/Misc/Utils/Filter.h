#pragma once

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

			NEXUS_ENGINE_API bool FilterObject(NxFr::StringView Substring, NxFr::GUID Id, NxFr::StringId Type);
			NEXUS_ENGINE_API bool FilterObject(const Object& Instance);

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
