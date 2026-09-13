#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Element.h"

namespace NxEn
{
	namespace GUI
	{
		class NX_ENGINE_API Panel : public Element
		{
		public:
			NX_OBJECT(Panel)

			template<typename T>
			static T* Create(bool Initialize = true)
			{
				NxFr::Allocator::Scope _ = MemorySystem::GetAllocator(AllocatorType::Constant);

				T* Instance = new T();
				if (Initialize)
				{
					Instance->Panel::Initialize();
				}
				return static_cast<T*>(GUISystem::GetPanels().Register(T::GetClassType(), Instance));
			}

			Panel();
			virtual ~Panel();

			virtual void Draw() override;

		protected:
			virtual void OnInitialize() override;
		};
	}
}

