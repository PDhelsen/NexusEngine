#pragma once

#include "NexusEngine/Systems/Resources/Resource.h"

namespace NxEn
{
	class NX_ENGINE_API Blob : public Resource
	{
	public:
		Blob(NxFr::StringView Path);
		~Blob();

		NxFr::Buffer& GetBytes() { return Data; }

	protected:
		void Save(NxFr::StringView FilePath) override;
		void Load(NxFr::StringView FilePath) override;
		void Unload() override;

	private:
		NxFr::Buffer Data;
	};
}
