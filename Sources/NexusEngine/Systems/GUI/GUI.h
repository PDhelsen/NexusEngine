#pragma once

#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	namespace GUI
	{
		class Element : public Object
		{
			friend class GUISystem;

		public:
			NX_OBJECT_DECLARATION(NX_ENGINE_API, Element)

			NX_ENGINE_API Element();
			NX_ENGINE_API ~Element();

			NX_ENGINE_API void Show();
			NX_ENGINE_API void Hide();
			NX_ENGINE_API void Close();

			NX_ENGINE_API NxFr::StringView GetImGuiId() const { return ImGuiId; }
			NX_ENGINE_API bool IsManual() const { return Manual; }
			NX_ENGINE_API void SetManual(bool Manual) { this->Manual = Manual; }

		protected:
			NX_ENGINE_API virtual void OnInitialize() override;
			NX_ENGINE_API virtual void OnEnable() override;
			NX_ENGINE_API virtual void OnDisable() override;
			NX_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NX_ENGINE_API virtual void OnGui(float TimeStep) = 0;

			NX_ENGINE_API void UpdateImGuiId(NxFr::StringView Name);

		private:
			NxFr::String ImGuiId;
			bool Manual;
			bool WillClose;
		};

		class Panel : public Element
		{
		public:
			NX_OBJECT_DECLARATION(NX_ENGINE_API, Panel)

			template<typename T>
			static T* Create()
			{
				//NxFr::AllocatorContext Allocator(MemorySystem::GetAllocator(AllocatorType::General));

				T* Instance = Object::Create<T>(false);
				GUISystem::RegisterPanel(Instance);
				return Instance;
			}

			NX_ENGINE_API Panel();
			NX_ENGINE_API virtual ~Panel();

			NX_ENGINE_API Panel& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			NX_ENGINE_API Panel& SetTitle(NxFr::StringView Title);
			NX_ENGINE_API Panel& SetDock(NxFr::StringView Id);

			NX_ENGINE_API ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NX_ENGINE_API NxFr::StringView GetTitle() const { return Title; }

		protected:
			NX_ENGINE_API virtual void OnInitialize() override;
			NX_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NX_ENGINE_API virtual void OnGui(float TimeStep) { };

		private:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
			NxFr::String Dock;
		};

		class Menu : public Element
		{
			friend class GUISystem;

		public:
			enum class ItemMode
			{
				Callback, Toggle, Enum
			};

			struct Item
			{
				friend class Menu;

			public:
				NX_ENGINE_API static Item Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);

				NX_ENGINE_API bool operator==(const Item& Other) const;
				NX_ENGINE_API bool operator<=(const Item& Other) const;

				NX_ENGINE_API const NxFr::Delegate<void()>& GetCallback() const { return Callback; }
				NX_ENGINE_API NxFr::StringView GetPath() const { return Path; }
				NX_ENGINE_API int64 GetPriority() const { return Priority; }
				NX_ENGINE_API ItemMode GetMode() const { return Mode; }

			private:
				NX_ENGINE_API Item(const NxFr::Delegate<void()>& Callback, const NxFr::Delegate<bool()>& Validate, NxFr::StringView Path, int64 Priority, ItemMode Mode, uint64 Index, void* Data);

			private:
				NxFr::Delegate<void()> Callback;
				NxFr::Delegate<bool()> Validate;
				NxFr::String Path;
				int64 Priority;
				ItemMode Mode;
				uint64 Index;
				void* Data;
			};

			NX_OBJECT_DECLARATION(NX_ENGINE_API, Menu)

			NX_ENGINE_API Menu(bool Main = false);
			NX_ENGINE_API virtual ~Menu();

			NX_ENGINE_API Menu& AddMenuItem	(NxFr::StringView Path,																	const NxFr::Delegate<void()>& Callback,				int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			NX_ENGINE_API Menu& AddMenuToggle(NxFr::StringView Path, void* Toggle,													const NxFr::Delegate<void()>& Callback = nullptr,	int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			NX_ENGINE_API Menu& AddMenuEnum	(NxFr::StringView Path, void* Enum,		const NxFr::Array<NxFr::StringView>& Labels,	const NxFr::Delegate<void()>& Callback = nullptr,	int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			NX_ENGINE_API Menu& Remove(NxFr::StringView Path);
			NX_ENGINE_API Menu& Clear();

			NX_ENGINE_API const Item& GetMenuItem(uint64 Index = 0) const { return Items[Index]; }
			NX_ENGINE_API uint64 GetMenuItemCount() const { return Items.GetCount(); }

		protected:
			NX_ENGINE_API virtual void OnShutdown() override;
			NX_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NX_ENGINE_API virtual void OnGui(float TimeStep) { };

			void AppendItem(const Item& It);
			void RemoveItem(const Item& It);
			void DrawMenu(float TimeStep);
			void DrawItem(const Item& It, const NxFr::List<NxFr::StringView>& Sections, uint64 Depth) const;

		private:
			NxFr::List<Item> Items;
			NxFr::Dictionary<NxFr::GUID, NxFr::String> Labels;
			bool Main;
		};

		class Popup : public Element
		{
			struct Item
			{
				NxFr::Delegate<void()> Callback;
				NxFr::String Label;
			};

		public:
			NX_OBJECT_DECLARATION(NX_ENGINE_API, Popup)

			NX_ENGINE_API Popup();
			NX_ENGINE_API virtual ~Popup();

			NX_ENGINE_API Popup& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			NX_ENGINE_API Popup& SetTitle(NxFr::StringView Title);
			NX_ENGINE_API Popup& SetMessage(NxFr::StringView Message);
			NX_ENGINE_API Popup& AddButton(NxFr::StringView Label);
			NX_ENGINE_API Popup& AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback);
			NX_ENGINE_API Popup& Clear();

			NX_ENGINE_API ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NX_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
			NX_ENGINE_API NxFr::StringView GetMessage() const { return Message; }
			NX_ENGINE_API NxFr::StringView GetButton(uint64 Index = 0) const { return Callbacks[Index].Label; }
			NX_ENGINE_API uint64 GetButtonCount() const { return Callbacks.GetCount(); }

		protected:
			NX_ENGINE_API virtual void OnInitialize() override;
			NX_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NX_ENGINE_API virtual void OnGui(float TimeStep) { };

		private:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::List<Item> Callbacks;
		};

		class ProgressBar : public Element
		{
		public:
			NX_OBJECT_DECLARATION(NX_ENGINE_API, ProgressBar)

			NX_ENGINE_API ProgressBar();
			NX_ENGINE_API virtual ~ProgressBar();

			NX_ENGINE_API ProgressBar& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			NX_ENGINE_API ProgressBar& SetTitle(NxFr::StringView Title);
			NX_ENGINE_API ProgressBar& SetMessage(NxFr::StringView Message);
			NX_ENGINE_API ProgressBar& SetCallback(const NxFr::Delegate<void()>& Callback);
			NX_ENGINE_API ProgressBar& SetProgress(float Progress);

			NX_ENGINE_API ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NX_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
			NX_ENGINE_API NxFr::StringView GetMessage() const { return Message; }
			NX_ENGINE_API float GetProgress() const { return Progress; }

		protected:
			NX_ENGINE_API virtual void OnInitialize() override;
			NX_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NX_ENGINE_API virtual void OnGui(float TimeStep) { };

			float ComputePercentage(float TimeStep);

		private:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::Delegate<void()> Callback;
			float Progress;
		};

		class Window : public NxEn::GUI::Element
		{
		public:
			NX_OBJECT_DECLARATION(NX_ENGINE_API, Window)

			NX_ENGINE_API Window();
			NX_ENGINE_API ~Window();

			NX_ENGINE_API Menu& GetMenu() { return MainMenu; }

		protected:
			NX_ENGINE_API void OnInitialize() override;
			NX_ENGINE_API void OnShutdown() override;
			NX_ENGINE_API void OnEnable() override;
			NX_ENGINE_API void OnDisable() override;
			NX_ENGINE_API void OnGui(float TimeStep) override;

		private:
			ImGuiWindowFlags GuiFlags;
			Menu MainMenu;
		};
	}
}
