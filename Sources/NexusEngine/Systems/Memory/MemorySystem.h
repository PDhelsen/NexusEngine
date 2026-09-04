#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"

namespace NxEn
{
	enum class AllocatorType
	{
		System,
		General,
		Temp,
		Constant,
		Fixed,
		Managed,
		COUNT
	};

	class NX_ENGINE_API MemorySystem : public System
	{
	public:
		NX_OBJECT(MemorySystem)

		static NxFr::Allocator* GetAllocator(AllocatorType Type);

		MemorySystem();
		~MemorySystem();

		void Defragment(float Budget = 0.0f);

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void ApplySettings();
		void DefragmentManagedAllocators(float Budget);
		void ClearTempAllocators();
	};
}
