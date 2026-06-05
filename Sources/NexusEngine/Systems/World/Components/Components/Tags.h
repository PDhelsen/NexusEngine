#pragma once

#include "NexusEngine/Systems/World/Components/Component.h"

namespace NxEn
{
	class NX_ENGINE_API Tags : public Component
	{
	public:
		NX_COMPONENT_DECLARATION(Tags)

		Tags();
		~Tags();

		void Append(NxFr::StringView Tag);
		void Append(const NxFr::Collection<NxFr::StringView>& Tags);
		void Remove(NxFr::StringView Tag);
		void Remove(const NxFr::Collection<NxFr::StringView>& Tags);
		bool Contains(NxFr::StringView Tag);
		bool Contains(const NxFr::Collection<NxFr::StringView>& Tags, bool MatchAll);

		const NxFr::Set<NxFr::String>::I Begin() const;
		const NxFr::Set<NxFr::String>::I End() const;

		bool IsEmpty() const { return Items.IsEmpty(); }
		uint64 GetCount() const { return Items.GetCount(); }

	protected:
		void OnDraw() override;
		void OnClone(const Object& Other) override;
		void OnSerialize(YAML::Node& Node) const override;
		void OnDeserialize(const YAML::Node& Node) override;

	private:
		NxFr::Set<NxFr::String> Items;
	};
}

