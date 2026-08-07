#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Element.h"

namespace NxEn
{
	namespace GUI
	{
		class NX_ENGINE_API Popup : public Element
		{
		public:
			struct NX_ENGINE_API Item
			{
				NxFr::Delegate<void()> Callback;
				NxFr::String Label;
			};

			NX_OBJECT(Popup)

			Popup();
			virtual ~Popup();

			virtual void Draw() override;

			void SetMessage(NxFr::StringView Message);
			void AddButton(NxFr::StringView Label);
			void AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback);
			void Clear();

		protected:
			virtual void OnInitialize() override;
			virtual void OnShutdown() override;

		private:
			NxFr::String Message;
			NxFr::List<Item> Items;
		};
	}
}

