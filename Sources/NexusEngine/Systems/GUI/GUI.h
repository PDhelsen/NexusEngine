#pragma once

#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	namespace GUI
	{
		class NX_ENGINE_API Element : public Object
		{
			friend class GUISystem;

		public:
			NX_OBJECT(Element)

			Element();
			~Element();

			void Show();
			void Hide();
			void Close();

			NxFr::StringView GetImGuiId() const { return ImGuiId; }
			bool IsManual() const { return Manual; }
			void SetManual(bool Manual) { this->Manual = Manual; }

		protected:
			virtual void OnInitialize() override;
			virtual void OnEnable() override;
			virtual void OnDisable() override;
			virtual void OnTick(float TimeStep = 0.0f) override;
			virtual void OnGui(float TimeStep) = 0;

			void UpdateImGuiId(NxFr::StringView Name);

		private:
			NxFr::String ImGuiId;
			bool Manual;
			bool WillClose;
		};

		class NX_ENGINE_API Panel : public Element
		{
		public:
			NX_OBJECT(Panel)

			template<typename T>
			static T* Create()
			{
				//NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

				T* Instance = new T();
				Instance->Initialize();
				GUISystem::RegisterPanel(Instance);
				return Instance;
			}

			Panel();
			virtual ~Panel();

			Panel& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			Panel& SetTitle(NxFr::StringView Title);
			Panel& SetDock(NxFr::StringView Id);

			ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NxFr::StringView GetTitle() const { return Title; }

		protected:
			virtual void OnInitialize() override;
			virtual void OnTick(float TimeStep = 0.0f) override;
			virtual void OnGui(float TimeStep) { };

		private:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
			NxFr::String Dock;
		};

		class NX_ENGINE_API Menu : public Element
		{
			friend class GUISystem;

		public:
			enum class ItemMode
			{
				Callback, Toggle, Enum
			};

			struct NX_ENGINE_API Item
			{
				friend class Menu;

			public:
				static Item Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);

				bool operator==(const Item& Other) const;
				bool operator<=(const Item& Other) const;

				const NxFr::Delegate<void()>& GetCallback() const { return Callback; }
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

			Menu(bool Main = false);
			virtual ~Menu();

			Menu& AddMenuItem	(NxFr::StringView Path,																	const NxFr::Delegate<void()>& Callback,				int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			Menu& AddMenuToggle(NxFr::StringView Path, void* Toggle,													const NxFr::Delegate<void()>& Callback = nullptr,	int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			Menu& AddMenuEnum	(NxFr::StringView Path, void* Enum,		const NxFr::Array<NxFr::StringView>& Labels,	const NxFr::Delegate<void()>& Callback = nullptr,	int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			Menu& Remove(NxFr::StringView Path);
			Menu& Clear();

			const Item& GetMenuItem(uint64 Index = 0) const { return Items[Index]; }
			uint64 GetMenuItemCount() const { return Items.GetCount(); }

		protected:
			virtual void OnShutdown() override;
			virtual void OnTick(float TimeStep = 0.0f) override;
			virtual void OnGui(float TimeStep) { };

			void AppendItem(const Item& It);
			void RemoveItem(const Item& It);
			void DrawMenu(float TimeStep);
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

			Popup& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			Popup& SetTitle(NxFr::StringView Title);
			Popup& SetMessage(NxFr::StringView Message);
			Popup& AddButton(NxFr::StringView Label);
			Popup& AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback);
			Popup& Clear();

			ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NxFr::StringView GetTitle() const { return Title; }
			NxFr::StringView GetMessage() const { return Message; }
			NxFr::StringView GetButton(uint64 Index = 0) const { return Callbacks[Index].Label; }
			uint64 GetButtonCount() const { return Callbacks.GetCount(); }

		protected:
			virtual void OnInitialize() override;
			virtual void OnTick(float TimeStep = 0.0f) override;
			virtual void OnGui(float TimeStep) { };

		private:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::List<Item> Callbacks;
		};

		class NX_ENGINE_API ProgressBar : public Element
		{
		public:
			NX_OBJECT(ProgressBar)

			ProgressBar();
			virtual ~ProgressBar();

			ProgressBar& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			ProgressBar& SetTitle(NxFr::StringView Title);
			ProgressBar& SetMessage(NxFr::StringView Message);
			ProgressBar& SetCallback(const NxFr::Delegate<void()>& Callback);
			ProgressBar& SetProgress(float Progress);

			ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NxFr::StringView GetTitle() const { return Title; }
			NxFr::StringView GetMessage() const { return Message; }
			float GetProgress() const { return Progress; }

		protected:
			virtual void OnInitialize() override;
			virtual void OnTick(float TimeStep = 0.0f) override;
			virtual void OnGui(float TimeStep) { };

			float ComputePercentage(float TimeStep);

		private:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
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

			Menu& GetMenu() { return MainMenu; }

		protected:
			void OnInitialize() override;
			void OnShutdown() override;
			void OnEnable() override;
			void OnDisable() override;
			void OnGui(float TimeStep) override;

		private:
			ImGuiWindowFlags GuiFlags;
			Menu MainMenu;
		};
	}
}
