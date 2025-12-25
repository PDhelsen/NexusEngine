#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"

namespace NxEd
{
	class HierarchyItem : public NxEn::Object
	{
		friend class HierarchyPanel;

	public:
		NEXUS_OBJECT_DECLARATION(NEXUS_EDITOR_API, HierarchyItem)

		NEXUS_EDITOR_API HierarchyItem(NxFr::Handle<NxEn::GameObject> GameObject);
		NEXUS_EDITOR_API virtual ~HierarchyItem();

		NEXUS_EDITOR_API NxFr::Handle<NxEn::GameObject> GetGameObject() { return GameObject; }
		NEXUS_EDITOR_API NxFr::StringView GetImGuiText() { EnsureImGuiText(); return ImGuiText; }
		NEXUS_EDITOR_API bool IsOpen() const { return Expanded; }
		NEXUS_EDITOR_API bool IsSelected() const { return Selected; }

	private:
		void EnsureImGuiText();

	private:
		NxFr::Handle<NxEn::GameObject> GameObject;
		NxFr::String ImGuiText;
		bool Expanded;
		bool Selected;

		NxFr::StringView Name;
		NxFr::GUID Reference;
	};
}
