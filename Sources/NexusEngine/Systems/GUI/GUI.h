#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object.h"

namespace NxEn
{
	namespace GUI
	{
		class Element : public Object
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Element)

			NEXUS_ENGINE_API Element(bool Manual);
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

		private:
			bool Manual;
		};

		class Panel : public Element
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Panel)

			NEXUS_ENGINE_API Panel();
			NEXUS_ENGINE_API virtual ~Panel();

			NEXUS_ENGINE_API Panel& SetTitle(NxFr::StringView Title);
			NEXUS_ENGINE_API Panel& SetPanelFlag(ImGuiWindowFlags_ PanelFlags);

			NEXUS_ENGINE_API ImGuiWindowFlags GetPanelFlags() const { return PanelFlags; }
			NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep);

		protected:
			ImGuiWindowFlags PanelFlags;
			NxFr::String Title;
		};

		class Popup : public Element
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Popup)

			NEXUS_ENGINE_API Popup();
			NEXUS_ENGINE_API virtual ~Popup();

			NEXUS_ENGINE_API Popup& SetTitle(NxFr::StringView Title);
			NEXUS_ENGINE_API Popup& SetMessage(NxFr::StringView Message);
			NEXUS_ENGINE_API Popup& AddButton(NxFr::StringView Label, const NxFr::Delegate<void()>& Callback);

			NEXUS_ENGINE_API ImGuiWindowFlags GetPanelFlags() const { return PanelFlags; }
			NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
			NEXUS_ENGINE_API NxFr::StringView GetMessage() const { return Message; }
			NEXUS_ENGINE_API NxFr::StringView GetButton(uint64 Index = 0) const { return Callbacks[Index].GetFirst(); }
			NEXUS_ENGINE_API uint64 GetButtonCount() const { return Callbacks.GetCount(); }

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep);

		protected:
			ImGuiWindowFlags PanelFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::List<NxFr::Tuple<NxFr::String, NxFr::Delegate<void()>>> Callbacks;
		};

		class ProgressBar : public Element
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, ProgressBar)

			NEXUS_ENGINE_API ProgressBar();
			NEXUS_ENGINE_API virtual ~ProgressBar();

			NEXUS_ENGINE_API ProgressBar& SetTitle(NxFr::StringView Title);
			NEXUS_ENGINE_API ProgressBar& SetMessage(NxFr::StringView Message);
			NEXUS_ENGINE_API ProgressBar& SetCallback(NxFr::Delegate<void()> Callback);
			NEXUS_ENGINE_API ProgressBar& SetProgress(float Progress);

			NEXUS_ENGINE_API ImGuiWindowFlags GetPanelFlags() const { return PanelFlags; }
			NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
			NEXUS_ENGINE_API NxFr::StringView GetMessage() const { return Message; }
			NEXUS_ENGINE_API float GetProgress() const { return Progress; }

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep);

			NEXUS_ENGINE_API float ComputePercentage(float TimeStep);

		protected:
			ImGuiWindowFlags PanelFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::Delegate<void()> Callback;
			float Progress;
		};
	}
}
