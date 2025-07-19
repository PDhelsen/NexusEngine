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

		protected:
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnEnable() override;
			NEXUS_ENGINE_API virtual void OnDisable() override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) = 0;
		};

		class Panel : public Element
		{
		public:
			NEXUS_OBJECT_DECLARATION(NEXUS_ENGINE_API, Panel)

			NEXUS_ENGINE_API Panel();
			NEXUS_ENGINE_API virtual ~Panel();

			NEXUS_ENGINE_API Panel& SetTitle(NxFr::StringView Title);
			NEXUS_ENGINE_API Panel& SetPanelFlag(ImGuiWindowFlags_ PanelFlags);

			NEXUS_ENGINE_API NxFr::StringView GetTitle() const { return Title; }
			NEXUS_ENGINE_API ImGuiWindowFlags GetPanelFlags() const { return PanelFlags; }

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) = 0;

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

		protected:
			NEXUS_ENGINE_API virtual void OnInitialize() override;
			NEXUS_ENGINE_API virtual void OnTick(float TimeStep = 0.0f) override;
			NEXUS_ENGINE_API virtual void OnGui(float TimeStep) { };

			NEXUS_ENGINE_API void Close();

		protected:
			ImGuiWindowFlags PanelFlags;
			NxFr::String Title;
			NxFr::String Message;
			NxFr::List<NxFr::Tuple<NxFr::String, NxFr::Delegate<void()>>> Callbacks;
		};
	}
}
