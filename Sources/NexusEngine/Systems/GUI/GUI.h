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
			NxFr::String Title;
			ImGuiWindowFlags PanelFlags;
		};
	}
}
