#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	namespace Edit
	{
		struct Context
		{
			inline static NxFr::StringId Dummy = "Dummy"_Sid;

			NEXUS_EDITOR_API Context(NxFr::StringId Id);
			NEXUS_EDITOR_API ~Context();

			template<typename T>
			inline bool IsBinded(const T& Callback) const
			{
				return !Callback.IsNull();
			}

			NEXUS_EDITOR_API inline NxEn::Object* TryGet(NxFr::GUID Target, NxFr::StringId Ctx = 0) const;

			const NxFr::StringId Id;

			NxFr::Delegate<NxEn::Object* (NxFr::GUID)> Convert;
			NxFr::Delegate<NxFr::List<NxEn::Object*>()> Select;
			NxFr::Delegate<void(NxEn::Object*)> Rename;
			NxFr::Delegate<void(NxEn::Object*)> Delete;
			NxFr::Delegate<void(NxEn::Object*)> Paste;
		};

		struct Info
		{
			Info(NxFr::StringId Context, NxFr::GUID Target, NxEn::Object* Data);
			~Info();

			bool operator==(const Info& Other) const;
			bool operator!=(const Info& Other) const;

			NxFr::StringId Context;
			NxFr::GUID Target;
			NxEn::Object* Data;
		};
	}
}
