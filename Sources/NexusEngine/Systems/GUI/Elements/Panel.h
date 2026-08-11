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
			static T* Create()
			{
				T* Instance = new T();
				Instance->Initialize();
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

