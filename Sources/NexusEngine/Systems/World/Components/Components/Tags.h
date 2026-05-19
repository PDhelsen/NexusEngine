#pragma once

#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	class Tags : public Component
	{
	public:
		NX_COMPONENT_DECLARATION(NX_ENGINE_API, Tags)

		NX_ENGINE_API Tags();
		NX_ENGINE_API ~Tags();

		NX_ENGINE_API void Append(NxFr::StringView Tag);
		NX_ENGINE_API void Append(const NxFr::Collection<NxFr::StringView>& Tags);
		NX_ENGINE_API void Remove(NxFr::StringView Tag);
		NX_ENGINE_API void Remove(const NxFr::Collection<NxFr::StringView>& Tags);
		NX_ENGINE_API bool Contains(NxFr::StringView Tag);
		NX_ENGINE_API bool Contains(const NxFr::Collection<NxFr::StringView>& Tags, bool MatchAll);

		NX_ENGINE_API const NxFr::Set<NxFr::String>::I Begin() const;
		NX_ENGINE_API const NxFr::Set<NxFr::String>::I End() const;

		NX_ENGINE_API bool IsEmpty() const { return Items.IsEmpty(); }
		NX_ENGINE_API uint64 GetCount() const { return Items.GetCount(); }

	protected:
		NX_ENGINE_API void OnGui(float TimeStep) override;
		NX_ENGINE_API void OnClone(const Object& Other) override;
		NX_ENGINE_API void OnSave(YAML::Node& Node) override;
		NX_ENGINE_API void OnLoad(const YAML::Node& Node) override;

	private:
		NxFr::Set<NxFr::String> Items;
	};
}

