#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Systems/System.h"
#include "NexusEngine/Systems/Memory/HandleManager.h"
#include "NexusEngine/Systems/Memory/Allocator.h"

namespace NxEn
{
	class NX_ENGINE_API MemorySystem : public System
	{
	public:
		NX_OBJECT(MemorySystem)

		static NxEn::HandleManager* GetHandleManager();
		static NxEn::Allocator* GetAllocator(AllocatorType Type);

		MemorySystem();
		~MemorySystem();

		void Defragment(float Budget = 0.0f);

	protected:
		void OnInitialize() override;
		void OnShutdown() override;
		void OnTick(float TimeStep = 0.0f) override;

	private:
		void DefragmentManagedAllocators(float Budget = 0.0f);
		void ClearTempAllocators();

		TimeManager* Time;
		NxFr::Vector2i Defragmentation;
	};
}
