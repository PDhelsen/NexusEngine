#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Systems/GUI/Element.h"

namespace NxEn
{
	namespace GUI
	{
		class NX_ENGINE_API Menu : public Element
		{
		public:
			enum class ItemMode
			{
				Callback, Toggle, Enum
			};

			struct NX_ENGINE_API Item
			{
				Item(const NxFr::Delegate<void()>& Callback, const NxFr::Delegate<bool()>& Validate, NxFr::StringView Path, int64 Priority, ItemMode Mode, uint64 Index, void* Data);

				bool operator==(const Item& Other) const;
				bool operator<=(const Item& Other) const;

				NxFr::Delegate<void()> Callback;
				NxFr::Delegate<bool()> Validate;
				NxFr::String Path;
				int64 Priority;
				ItemMode Mode;
				uint64 Index;
				void* Data;
			};

			NX_OBJECT(Menu)

			static const Menu::Item* Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			static int64 MainMenuPriority(NxFr::StringView Path, int64 Priority);

			Menu();
			virtual ~Menu();

			virtual void Draw() override;

			void AddMenuItem(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			void AddMenuToggle(NxFr::StringView Path, void* Toggle, const NxFr::Delegate<void()>& Callback = nullptr, int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			void AddMenuEnum(NxFr::StringView Path, void* Enum, const NxFr::Array<NxFr::StringView>& Labels, const NxFr::Delegate<void()>& Callback = nullptr, int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			void Remove(NxFr::StringView Path);
			void Clear();

		protected:
			virtual void OnInitialize() override;
			virtual void OnShutdown() override;

			void AppendItem(const Item& It);
			void RemoveItem(const Item& It);
			void DrawMenu();
			void DrawItem(const Item& It, const NxFr::List<NxFr::StringView>& Sections, uint64 Depth) const;

		private:
			NxFr::List<Item> Items;
			NxFr::Dictionary<NxFr::GUID, NxFr::String> Labels;
		};
	}
}

