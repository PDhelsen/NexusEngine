#pragma once

#include "NexusEditor/Core/NexusEditorCore.h"
#include "NexusEngine/Misc/GUI/TreePanel.h"

namespace NxEd
{
	class NX_EDITOR_API AssetsBrowserItem : public NxEn::Rework::TreeItem
	{
		friend class AssetsBrowser;

	public:
		NX_OBJECT(AssetsBrowserItem)

		NxFr::GUID GetId() const override { return Id; }
		NxFr::StringView GetName() const override { return GetPrettyName(); }
		NxFr::StringView GetLabel() const override { return Label; }
		NxFr::StringView GetDescription() const override { return Path; }
		NxFr::StringId GetType() const override { return Type; }

		NxFr::StringView GetTargetPath() const { return Path; }
		NxFr::StringView GetTargetName() const { return NxFr::Path::GetNameAndExtension(Path); }
		NxFr::StringView GetPrettyPath() const { return NxFr::Path::GetPathWithoutExtension(Path); }
		NxFr::StringView GetPrettyName() const { return NxFr::Path::GetName(Path); }

		NxFr::GUID GetParent() const override { return Parent; }
		NxFr::GUID GetPrevious() const override { return Previous; }
		NxFr::GUID GetNext() const override { return Next; }
		NxFr::GUID GetChild() const override { return Child; }

		bool IsOpen() const override { return Opened; }
		void Open(bool State) override { Opened = State; }

		bool Compare(const TreeItem& Other) const override;

	protected:
		AssetsBrowserItem();
		virtual ~AssetsBrowserItem();

		virtual void OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath) = 0;
		virtual void OnMove(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) = 0;
		virtual void OnDuplicate(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) = 0;
		virtual void OnDelete(NxFr::StringView CurrentPath) = 0;

		virtual NxFr::StringView GetPrefix() const = 0;

	private:
		NxFr::GUID Id;
		NxFr::String Label;
		NxFr::String Path;
		NxFr::StringId Type;
		bool Opened;

		NxFr::GUID Parent;
		NxFr::GUID Previous;
		NxFr::GUID Next;
		NxFr::GUID Child;
	};

	class AssetsBrowserItemDirectory : public AssetsBrowserItem
	{
	public:
		NX_OBJECT(AssetsBrowserItemDirectory)

	protected:
		void OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath) override;
		void OnMove(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) override;
		void OnDuplicate(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) override;
		void OnDelete(NxFr::StringView CurrentPath) override;

		NxFr::StringView GetPrefix() const override { return "D"; }
	};

	class AssetsBrowserItemContent : public AssetsBrowserItem
	{
	public:
		NX_OBJECT(AssetsBrowserItemContent)

	protected:
		void OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath) override;
		void OnMove(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) override;
		void OnDuplicate(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) override;
		void OnDelete(NxFr::StringView CurrentPath) override;

		NxFr::StringView GetPrefix() const override { return "C"; };
	};

	class AssetsBrowserItemAsset : public AssetsBrowserItem
	{
	public:
		NX_OBJECT(AssetsBrowserItemAsset)

	protected:
		void OnCreate(NxFr::StringId Type, NxFr::StringView TargetPath) override;
		void OnMove(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) override;
		void OnDuplicate(NxFr::StringView CurrentPath, NxFr::StringView TargetPath) override;
		void OnDelete(NxFr::StringView CurrentPath) override;

		NxFr::StringView GetPrefix() const override { return "A"; };
	};
}
