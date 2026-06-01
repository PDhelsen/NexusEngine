#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	namespace Utils
	{
		class NX_ENGINE_API Filter
		{
		public:
			Filter(NxFr::StringView Query = "");
			~Filter();

			void Configure();

			NxFr::Set<Object*> FilterObjects(const NxFr::Collection<Object*> Instances);
			bool FilterObject(const Object& Instance);
			bool FilterInstance(NxFr::StringView Substring, NxFr::GUID Id, NxFr::StringId Type);

			bool IsEmpty() const { return Query.IsEmpty(); }
			NxFr::String& GetQuery() { return Query; }

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
