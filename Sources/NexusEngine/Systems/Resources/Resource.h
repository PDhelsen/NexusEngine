#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	class NX_ENGINE_API Resource : public Object
	{
		friend class ResourcesSystem;

	public:
		NX_OBJECT(Object)

		Resource(NxFr::StringView Path);
		virtual ~Resource();

		NxFr::GUID GetId() const { return NxFr::Hash<>::HashObject(Path); }
		NxFr::StringView GetName() const { return NxFr::Path::GetName(Path); }
		NxFr::StringView GetPath() const { return Path; }

	protected:
		virtual void OnSave(NxFr::StringView Path) const {};
		virtual void OnLoad(NxFr::StringView Path) {};
		virtual void OnUnload() {};

	private:
		NxFr::StringView Path;
	};
}


