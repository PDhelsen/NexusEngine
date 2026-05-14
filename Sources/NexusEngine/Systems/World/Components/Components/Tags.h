#pragma once

#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	class Tags : public Component
	{
	public:
		NEXUS_COMPONENT_DECLARATION(NEXUS_ENGINE_API, Tags)

		NEXUS_ENGINE_API Tags();
		NEXUS_ENGINE_API ~Tags();

		NEXUS_ENGINE_API void Append(NxFr::StringView Tag);
		NEXUS_ENGINE_API void Append(const NxFr::Collection<NxFr::StringView>& Tags);
		NEXUS_ENGINE_API void Remove(NxFr::StringView Tag);
		NEXUS_ENGINE_API void Remove(const NxFr::Collection<NxFr::StringView>& Tags);
		NEXUS_ENGINE_API bool Contains(NxFr::StringView Tag);
		NEXUS_ENGINE_API bool Contains(const NxFr::Collection<NxFr::StringView>& Tags, bool MatchAll);

		NEXUS_ENGINE_API const NxFr::Set<NxFr::String>::I Begin() const;
		NEXUS_ENGINE_API const NxFr::Set<NxFr::String>::I End() const;

		NEXUS_ENGINE_API bool IsEmpty() const { return Items.IsEmpty(); }
		NEXUS_ENGINE_API uint64 GetCount() const { return Items.GetCount(); }

	protected:
		NEXUS_ENGINE_API void OnGui(float TimeStep) override;
		NEXUS_ENGINE_API void OnClone(const Object& Other) override;
		NEXUS_ENGINE_API void OnSave(YAML::Node& Node) override;
		NEXUS_ENGINE_API void OnLoad(const YAML::Node& Node) override;

	private:
		NxFr::Set<NxFr::String> Items;
	};
}

