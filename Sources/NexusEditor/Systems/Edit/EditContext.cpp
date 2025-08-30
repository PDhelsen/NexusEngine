#include "NexusEditor/Systems/Edit/EditContext.h"

namespace NxEd
{
	namespace Edit
	{
		Context::Context(NxFr::StringId Id)
			: Id(Id)
		{
		}

		Context::~Context()
		{
		}

		NxEn::Object* Context::TryGet(NxFr::GUID Target, NxFr::StringId Ctx) const
		{
			if (Ctx.GetId() != 0 && Ctx != Id)
			{
				return nullptr;
			}

			if (IsBinded(Convert))
			{
				return nullptr;
			}

			return Convert.Invoke(Target);
		}

		Info::Info(NxFr::StringId Context, NxFr::GUID Target, NxEn::Object* Data)
			: Context(Context), Target(Target), Data(Data)
		{
		}

		Info::~Info()
		{
		}

		bool Info::operator==(const Info& Other) const
		{
			return Context == Other.Context && Target == Other.Target && Data == Other.Data;
		}

		bool Info::operator!=(const Info& Other) const
		{
			return !(*this == Other);
		}
	}
}
