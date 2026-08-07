#pragma once

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
			RecycleInsteadOfClose = 1 << 3
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
			NX_OBJECT(Element)

			Element();
			~Element();

			void Show();
			void Hide();
			void Close();

			NxFr::GUID GetId() const override { return Id; }
			NxFr::StringView GetName() const override { return Name; }
			NxFr::StringView GetNamedId() const { return NamedId; }
			void SetNameId(NxFr::StringView Name, NxFr::GUID Id = 0);
			bool IsAutoDraw() const { return GetGuiFlag(ElementFlags::AutoDraw); }
			void SetAutoDraw(bool Auto) { SetGuiFlag(ElementFlags::AutoDraw, Auto); }

		protected:
			virtual void OnInitialize() override;
			virtual void OnEnable() override;
			virtual void OnDisable() override;

			NX_FORCE_INLINE bool GetGuiFlag(ElementFlags Flag) const;
			NX_FORCE_INLINE void SetGuiFlag(ElementFlags Flag, bool Value);
			NX_FORCE_INLINE bool GetImGuiFlag(uint64 Flag) const;
			NX_FORCE_INLINE void SetImGuiFlag(uint64 Flag, bool Value);
			NX_FORCE_INLINE uint64 GetImGuiFlags() const;
			NX_FORCE_INLINE void SetImGuiFlags(uint64 Flags);

		private:
			NxFr::GUID Id;
			NxFr::StringView Name;
			NxFr::String NamedId;
			ElementFlags GuiFlags;
			uint64 ImGuiFlags;
		};

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

			Panel& SetDock(NxFr::StringView Id);

		protected:
			virtual void OnInitialize() override;

		private:
			NxFr::String Dock;
		};

		class NX_ENGINE_API Menu : public Element
		{
		public:
			enum class ItemMode
			{
				Callback, Toggle, Enum
			};

			struct NX_ENGINE_API Item
			{
				friend class Menu;

			public:
				static const Item* Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);

				bool operator==(const Item& Other) const;
				bool operator<=(const Item& Other) const;

				const NxFr::Delegate<void()>& GetCallback() const { return Callback; }
				const NxFr::Delegate<bool()>& GetValidate() const { return Validate; }
				NxFr::StringView GetPath() const { return Path; }
				int64 GetPriority() const { return Priority; }
				ItemMode GetMode() const { return Mode; }

			private:
				Item(const NxFr::Delegate<void()>& Callback, const NxFr::Delegate<bool()>& Validate, NxFr::StringView Path, int64 Priority, ItemMode Mode, uint64 Index, void* Data);

			private:
				NxFr::Delegate<void()> Callback;
				NxFr::Delegate<bool()> Validate;
				NxFr::String Path;
				int64 Priority;
				ItemMode Mode;
				uint64 Index;
				void* Data;
			};

			NX_OBJECT(Menu)

			static int64 ComputePriority(NxFr::StringView Path, int64 Priority);

			Menu(bool Main = false);
			virtual ~Menu();

			virtual void Draw() override;

			Menu& AddMenuItem	(NxFr::StringView Path,																	const NxFr::Delegate<void()>& Callback,				int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			Menu& AddMenuToggle	(NxFr::StringView Path, void* Toggle,													const NxFr::Delegate<void()>& Callback = nullptr,	int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			Menu& AddMenuEnum	(NxFr::StringView Path, void* Enum,		const NxFr::Array<NxFr::StringView>& Labels,	const NxFr::Delegate<void()>& Callback = nullptr,	int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			Menu& Remove(NxFr::StringView Path);
			Menu& Clear();

			const Item& GetMenuItem(uint64 Index = 0) const { return Items[Index]; }
			uint64 GetMenuItemCount() const { return Items.GetCount(); }

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
			bool Main;
		};

		class NX_ENGINE_API Popup : public Element
		{
			struct Item
			{
				NxFr::Delegate<void()> Callback;
				NxFr::String Label;
			};

		public:
			NX_OBJECT(Popup)

			Popup();
			virtual ~Popup();

			virtual void Draw() override;

			Popup& SetMessage(NxFr::StringView Message);
			Popup& AddButton(NxFr::StringView Label);
			Popup& AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback);
			Popup& Clear();

			NxFr::StringView GetMessage() const { return Message; }
			NxFr::StringView GetButton(uint64 Index = 0) const { return Callbacks[Index].Label; }
			uint64 GetButtonCount() const { return Callbacks.GetCount(); }

		protected:
			virtual void OnInitialize() override;
			virtual void OnShutdown() override;

		private:
			NxFr::String Message;
			NxFr::List<Item> Callbacks;
		};

		class NX_ENGINE_API ProgressBar : public Element
		{
		public:
			NX_OBJECT(ProgressBar)

			ProgressBar();
			virtual ~ProgressBar();

			virtual void Draw() override;

			ProgressBar& SetMessage(NxFr::StringView Message);
			ProgressBar& SetCallback(const NxFr::Delegate<void()>& Callback);
			ProgressBar& SetProgress(float Progress);

			NxFr::StringView GetMessage() const { return Message; }
			float GetProgress() const { return Progress; }

		protected:
			virtual void OnInitialize() override;

			float ComputePercentage(float TimeStep);

		private:
			NxFr::String Message;
			NxFr::Delegate<void()> Callback;
			float Progress;
		};

		class NX_ENGINE_API Window : public NxEn::GUI::Element
		{
		public:
			NX_OBJECT(Window)

			Window();
			~Window();

			virtual void Draw() override;

			Menu& GetMenu() { return MainMenu; }

		protected:
			void OnInitialize() override;
			void OnShutdown() override;

		private:
			Menu MainMenu;
		};
	}
}
