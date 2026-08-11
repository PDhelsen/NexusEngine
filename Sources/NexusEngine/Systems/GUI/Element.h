#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	namespace GUI
	{
		enum class ElementFlags : uint8
		{
			None = 0,

			AutoDraw = 1 << 0,
			WillClose = 1 << 1,
			HideInsteadOfClose = 1 << 2,
			RecycleInsteadOfClose = 1 << 3,
			Main = 1 << 4,
		};
	}
}

NX_FLAG(NxEn::GUI::ElementFlags, uint8)

namespace NxEn
{
	namespace GUI
	{
		class NX_ENGINE_API Element : public Object
		{
		public:
			template<typename T>
			static T* Acquire(bool Enabled = true)
			{
				GUI::Element* Instance = Acquire(T::GetClassType());
				if (Instance == nullptr)
				{
					Instance = new T();
				}

				Instance->Initialize();
				Instance->SetEnabled(Enabled);
				return static_cast<T*>(Instance);
			}

			NX_OBJECT(Element)

			Element();
			~Element();

			virtual void Show();
			virtual void Hide();
			virtual void Close();

			virtual bool IsFocused() const;

			NxFr::GUID GetId() const override { return Id; }
			NxFr::StringView GetName() const override { return Name; }
			NxFr::StringView GetNamedId() const { return NamedId; }
			void SetNameId(NxFr::StringView Name, NxFr::GUID Id = 0);

			NX_FORCE_INLINE bool GetGuiFlag(ElementFlags Flag) const;
			NX_FORCE_INLINE void SetGuiFlag(ElementFlags Flag, bool Value);
			NX_FORCE_INLINE bool GetImGuiFlag(uint64 Flag) const;
			NX_FORCE_INLINE void SetImGuiFlag(uint64 Flag, bool Value);
			NX_FORCE_INLINE uint64 GetImGuiFlags() const;
			NX_FORCE_INLINE void SetImGuiFlags(uint64 Flags);

		protected:
			virtual void OnInitialize() override;
			virtual void OnEnable() override;
			virtual void OnDisable() override;

		private:
			static GUI::Element* Acquire(NxFr::StringId Type);

			NxFr::GUID Id;
			NxFr::StringView Name;
			NxFr::String NamedId;
			ElementFlags GuiFlags;
			uint64 ImGuiFlags;
		};
	}
}
