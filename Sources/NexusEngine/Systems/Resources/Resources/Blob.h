#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API Blob : public Resource
	{
	public:
		Blob(NxFr::StringView Path);
		~Blob();

		NxFr::Buffer& GetBytes() { return Data; }
		const NxFr::Buffer& GetBytes() const { return Data; }

	protected:
		void OnSave(NxFr::StringView Path) const override;
		void OnLoad(NxFr::StringView Path) override;
		void OnUnload() override;

	private:
		NxFr::Buffer Data;
	};
}
