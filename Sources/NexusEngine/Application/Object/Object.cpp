#include "NexusEngine/Core/NexusEnginePch.h"
#include "NexusEngine/Application/Object/Object.h"

namespace NxEn
{
	Object::Object()
		: Flags(ObjectFlags::None)
	{
	}

	Object::~Object()
	{
	}

	bool Object::operator==(const Object& Other) const
	{
		return Equals(Other.GetId());
	}

	bool Object::operator!=(const Object& Other) const
	{
		return !Equals(Other.GetId());
	}

	bool Object::Equals(NxFr::GUID Id) const
	{
		return GetId() == Id;
	}

	NxFr::GUID Object::GetId() const
	{
		return reinterpret_cast<NxFr::GUID>(this);
	}

	NxFr::StringView Object::GetName() const
	{
		return GetObjectType().GetString();
	}

	void Object::Initialize()
	{
		if (IsInitialized())
		{
			return;
		}

		OnInitialize();

		SetFlag(ObjectFlags::Initialized, true);
	}

	void Object::Shutdown()
	{
		if (!IsInitialized())
		{
			return;
		}

		OnShutdown();

		SetFlag(ObjectFlags::Initialized, false);
	}

	bool Object::IsInitialized() const
	{
		return GetFlag(ObjectFlags::Initialized);
	}

	void Object::UpdateHierarchy()
	{
		OnUpdateHierarchy();
	}

	bool Object::IsEnabled() const
	{
		return GetFlag(ObjectFlags::Enabled);
	}

	bool Object::IsEnabledInHierarchy() const
	{
		return GetFlag(ObjectFlags::EnabledInHierarchy);
	}

	void Object::SetEnabled(bool Enabled)
	{
		SetFlag(ObjectFlags::Enabled, Enabled);
		UpdateHierarchy();
	}

	void Object::Tick(float TimeStep)
	{
		if (!IsTicking())
		{
			return;
		}

		OnTick(TimeStep);
	}

	bool Object::IsTickable() const
	{
		return GetFlag(ObjectFlags::Tickable);
	}

	void Object::SetTickable(bool Tickable)
	{
		SetFlag(ObjectFlags::Tickable, Tickable);
	}

	bool Object::IsTicking() const
	{
		return IsTickable() && IsEnabledInHierarchy();
	}

	void Object::Draw()
	{
		OnDraw();
	}

	Object* Object::Clone() const
	{
		NX_ASSERT(false, Default, "Not Implemented");
		return nullptr;
	}

	void Object::Clone(Object* Other) const
	{
		Other->Clone(this);
	}

	void Object::Clone(const Object* Other)
	{
		OnClone(*Other);
	}

	YAML::Node Object::Save()
	{
		YAML::Node Node;
		OnSave(Node);
		return Node;
	}

	void Object::Load(const YAML::Node& Node)
	{
		OnLoad(Node);
	}

	void Object::Unload()
	{
		OnUnload();
	}

	void Object::PatchReferences()
	{
		OnPatchReferences();
	}

	NxFr::Array<NxFr::GUID> Object::GetDependencies()
	{
		NxFr::Set<NxFr::GUID> Ids;
		OnGetDependencies(Ids);
		return NxFr::ContainerUtility::ToArray<NxFr::GUID>(Ids);
	}

	void Object::OnInitialize()
	{

	}

	void Object::OnShutdown()
	{

	}

	void Object::OnUpdateHierarchy()
	{
		bool Enabled = IsEnabled();
		if (Enabled == IsEnabledInHierarchy())
		{
			return;
		}

		SetFlag(ObjectFlags::EnabledInHierarchy, Enabled);

		if (Enabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}

	void Object::OnEnable()
	{

	}

	void Object::OnDisable()
	{

	}

	void Object::OnTick(float TimeStep)
	{

	}

	void Object::OnDraw()
	{
	}

	void Object::OnClone(const Object& Other)
	{

	}

	void Object::OnSave(YAML::Node& Node)
	{

	}

	void Object::OnLoad(const YAML::Node& Node)
	{

	}

	void Object::OnUnload()
	{

	}

	void Object::OnPatchReferences()
	{

	}

	void Object::OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids)
	{

	}

	bool Object::GetFlag(ObjectFlags Flag) const
	{
		return NxFr::Enum::CheckFlag(Flags, Flag);
	}

	void Object::SetFlag(ObjectFlags Flag, bool Value)
	{
		Flags = NxFr::Enum::SetFlag(Flags, Flag, Value);
	}
}
