#pragma once

#include "NexusEngine/Application/Object.h"
#include "NexusEngine/Systems/GUI/Styles.h"

namespace NxEn
{
	namespace GUI
	{
		class Element;
		class Window;
		class Panel;
		class Menu;
		class Popup;
		class Progress;

		class Element : public Object
		{
			friend class GUISystem;

		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Element)

			NEXUS_ENGINE_API Element();
			NEXUS_ENGINE_API ~Element();

			NEXUS_ENGINE_API void Show();
			NEXUS_ENGINE_API void Hide();
			NEXUS_ENGINE_API void Close();

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnEnable() override;
			NEXUS_ENGINE_API virtual void OnDisable() override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) = 0;

			NEXUS_ENGINE_API bool IsManual() const { return Manual; }
			NEXUS_ENGINE_API void SetManual(bool Manual) { this->Manual = Manual; }

		private:
			bool Manual;
			bool WillClose;
		};

		class Window : public NxEn::GUI::Element
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Window)

			NEXUS_ENGINE_API Window();
			NEXUS_ENGINE_API ~Window();

		protected:
			NEXUS_ENGINE_API void OnInitialize() override;
			NEXUS_ENGINE_API void OnShutdown() override;
			NEXUS_ENGINE_API void OnTick(float TimeStep) override;
			NEXUS_ENGINE_API void OnEnable() override;
			NEXUS_ENGINE_API void OnDisable() override;
			NEXUS_ENGINE_API void OnGui(float TimeStep) override;

		private:
			ImGuiWindowFlags GuiFlags;
			Style Style;

			Menu* MainMenu;
		};

		class Panel : public Element
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Panel)

			template<typename T>
			static T* Create()
			{
				T* Instance = Object::Create<T>(false);
				GUISystem::RegisterPanel(Instance);
				return Instance;
			}

			NEXUS_ENGINE_API Panel();
			NEXUS_ENGINE_API virtual ~Panel();

			NEXUS_ENGINE_API Panel& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			NEXUS_ENGINE_API Panel& SetTitle(NxFr::StringView Title);

			NEXUS_ENGINE_API ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) { };

		protected:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
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
				NEXUS_ENGINE_API static Item Create(NxFr::StringView Path, const NxFr::Delegate<void()>& Callback, NxFr::StringView Shortcut = "", int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);

				NEXUS_ENGINE_API bool operator==(const Item& Other) const;
				NEXUS_ENGINE_API bool operator<=(const Item& Other) const;

				NEXUS_ENGINE_API const NxFr::Delegate<void()>& GetCallback() const { return Callback; }
				NEXUS_ENGINE_API NxFr::StringView GetPath() const { return Path; }
				NEXUS_ENGINE_API NxFr::StringView GetShortcut() const { return Shortcut; }
				NEXUS_ENGINE_API int64 GetPriority() const { return Priority; }
				NEXUS_ENGINE_API ItemMode GetMode() const { return Mode; }

			private:
				NEXUS_ENGINE_API Item(const NxFr::Delegate<void()>& Callback, const NxFr::Delegate<bool()>& Validate, NxFr::StringView Path, NxFr::StringView Shortcut, int64 Priority, ItemMode Mode, uint64 Index, void* Data);

			private:
				NxFr::Delegate<void()> Callback;
				NxFr::Delegate<bool()> Validate;
				NxFr::String Path;
				NxFr::String Shortcut;
				int64 Priority;
				ItemMode Mode;
				uint64 Index;
				void* Data;
			};

			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Menu)

			NEXUS_ENGINE_API Menu(bool Main = false);
			NEXUS_ENGINE_API virtual ~Menu();

			NEXUS_ENGINE_API Menu& AddMenuItem	(NxFr::StringView Path,																	const NxFr::Delegate<void()>& Callback,				NxFr::StringView Shortcut = "", int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			NEXUS_ENGINE_API Menu& AddMenuToggle(NxFr::StringView Path, void* Toggle,													const NxFr::Delegate<void()>& Callback = nullptr,	NxFr::StringView Shortcut = "", int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			NEXUS_ENGINE_API Menu& AddMenuEnum	(NxFr::StringView Path, void* Enum,		const NxFr::Array<NxFr::StringView>& Labels,	const NxFr::Delegate<void()>& Callback = nullptr,	NxFr::StringView Shortcut = "", int64 Priority = 0, const NxFr::Delegate<bool()>& Validate = nullptr);
			NEXUS_ENGINE_API Menu& Remove(NxFr::StringView Path);

			NEXUS_ENGINE_API const Item& GetMenuItem(uint64 Index = 0) const { return Items[Index]; }
			NEXUS_ENGINE_API uint64 GetMenuItemCount() const { return Items.GetCount(); }

		protected:
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) { };

			void AppendItem(const Item& It);
			void RemoveItem(const Item& It);
			void DrawMenu(float TimeStep);
			void DrawItem(const Item& It, const NxFr::List<NxFr::StringView>& Sections, uint64 Depth) const;

		protected:
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
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Popup)

			NEXUS_ENGINE_API Popup();
			NEXUS_ENGINE_API virtual ~Popup();

			NEXUS_ENGINE_API Popup& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			NEXUS_ENGINE_API Popup& SetTitle(NxFr::StringView Title);
			NEXUS_ENGINE_API Popup& SetMessage(NxFr::StringView Message);
			NEXUS_ENGINE_API Popup& AddButton(NxFr::StringView Label);
			NEXUS_ENGINE_API Popup& AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback);

			NEXUS_ENGINE_API ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
			NEXUS_ENGINE_API NxFr::StringView GetMessage() const { return Message; }
			NEXUS_ENGINE_API NxFr::StringView GetButton(uint64 Index = 0) const { return Callbacks[Index].Label; }
			NEXUS_ENGINE_API uint64 GetButtonCount() const { return Callbacks.GetCount(); }

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) { };

		protected:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::List<Item> Callbacks;
		};

		class ProgressBar : public Element
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, ProgressBar)

			NEXUS_ENGINE_API ProgressBar();
			NEXUS_ENGINE_API virtual ~ProgressBar();

			NEXUS_ENGINE_API ProgressBar& SetGuiFlag(ImGuiWindowFlags GuiFlags);
			NEXUS_ENGINE_API ProgressBar& SetTitle(NxFr::StringView Title);
			NEXUS_ENGINE_API ProgressBar& SetMessage(NxFr::StringView Message);
			NEXUS_ENGINE_API ProgressBar& SetCallback(const NxFr::Delegate<void()>& Callback);
			NEXUS_ENGINE_API ProgressBar& SetProgress(float Progress);

			NEXUS_ENGINE_API ImGuiWindowFlags GetGuiFlags() const { return GuiFlags; }
			NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
			NEXUS_ENGINE_API NxFr::StringView GetMessage() const { return Message; }
			NEXUS_ENGINE_API float GetProgress() const { return Progress; }

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) { };

			float ComputePercentage(float TimeStep);

		protected:
			ImGuiWindowFlags GuiFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::Delegate<void()> Callback;
			float Progress;
		};
	}
}
