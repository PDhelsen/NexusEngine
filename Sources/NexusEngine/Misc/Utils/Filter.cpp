#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Misc/Utils/Filter.h"

namespace NxEn
{
	namespace Utils
	{
		Filter::Filter(NxFr::StringView Query)
			: Query(Query), Filters(), Types(), Ids(), Names(), TypeAndString(), All()
		{
			Filters = NxFr::StringUtility::SplitAll(Query, " ");

			Types = Filters.GetCount();
			Ids = Filters.GetCount();
			Names = Filters.GetCount();
			TypeAndString = false;
			All = Query == "*";

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				Types[Index] = 0;
				Ids[Index] = 0;
				Names[Index] = false;

				if (NxFr::StringUtility::Start(Filters[Index], "t:"))
				{
					NxFr::StringView Substring = Filters[Index].Substring(2, Filters[Index].GetCount() - 2);
					Types[Index] = Substring;
					TypeAndString = true;
				}
				else if (NxFr::StringUtility::Start(Filters[Index], "id:"))
				{
					NxFr::StringView Substring = Filters[Index].Substring(3, Filters[Index].GetCount() - 3);
					Ids[Index] = NxFr::StringUtility::FromString<NxFr::GUID>(Substring);
				}
				else
				{
					Names[Index] = !NxFr::StringUtility::Contains(Filters[Index], NxFr::Path::SeparatorFolder);
				}
			}

			TypeAndString &= Filters.GetCount() > 1;
		}

		Filter::~Filter()
		{
		}

		NxFr::Set<Object*> Filter::FilterObjects(const NxFr::Collection<Object*> Instances)
		{
			NxFr::Set<Object*> Result;
			for (Object* It : Instances)
			{
				if (FilterObject(*It))
				{
					Result.Append(It);
				}
			}
			return Result;
		}

		bool Filter::FilterObject(const Object& Instance)
		{
			return FilterInstance(Instance.GetName(), Instance.GetId(), Instance.GetObjectType());
		}

		bool Filter::FilterInstance(NxFr::StringView Substring, NxFr::GUID Id, NxFr::StringId Type)
		{
			bool MatchId = false;
			bool MatchType = false;
			bool MatchString = false;

			for (uint64 Index = 0; Index < Filters.GetCount(); ++Index)
			{
				if (Names[Index])
				{
					Substring = NxFr::Path::GetName(Substring);
				}

				MatchId |= Ids[Index] != 0 && Id == Ids[Index];
				MatchType |= Types[Index].GetId() != 0 && Type == Types[Index];
				MatchString |= NxFr::StringUtility::Contains(Substring, Filters[Index]);
			}

			return All ||
				MatchId ||
				(MatchType && !TypeAndString) ||
				(MatchString && !TypeAndString) ||
				(MatchType && MatchString && TypeAndString);
		}
	}
}
