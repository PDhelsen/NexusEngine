#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"

#define NX_OBJECT(Type)                          \
    virtual NxFr::StringId GetObjectType() const \
    {                                            \
        return GetClassType();                   \
    }                                            \
                                                 \
    static NxFr::StringId GetClassType()         \
    {                                            \
        static NxFr::StringId T = #Type##_Sid;   \
        return T;                                \
    }

namespace NxEn
{
	enum class ObjectFlags : uint8
	{
		None = 0,

		Initialized = 1 << 0,
		Enabled = 1 << 1,
		EnabledInHierarchy = 1 << 2,
		Tickable = 1 << 3,
	};
}

NX_FLAG(NxEn::ObjectFlags, uint8)

namespace NxEn
{
	class NX_ENGINE_API Object
	{
	public:
		inline static const NxFr::GUID NullId = 0;

		Object();
		virtual ~Object();

		bool operator==(const Object& Other) const;
		bool operator!=(const Object& Other) const;
		bool Equals(NxFr::GUID Id) const;

		NX_OBJECT(Object)
		virtual NxFr::GUID GetId() const;
		virtual NxFr::StringView GetName() const;

		virtual void Initialize();
		virtual void Shutdown();
		virtual bool IsInitialized() const;

		virtual void UpdateHierarchy();
		virtual bool IsEnabled() const;
		virtual bool IsEnabledInHierarchy() const;
		virtual void SetEnabled(bool Enabled);

		virtual void Tick(float TimeStep = 0.0f);
		virtual bool IsTickable() const;
		virtual void SetTickable(bool Tickable);
		virtual bool IsTicking() const;

		virtual void Draw();

		virtual Object* Clone() const;
		virtual void Clone(Object* Other) const;
		virtual void Clone(const Object* Other);

		virtual NxFr::Yaml::Node Serialize() const;
		virtual void Deserialize(const NxFr::Yaml::Node& Node);
		virtual void Save(NxFr::StringView Path) const;
		virtual void Load(NxFr::StringView Path);
		virtual void Unload();

		virtual NxFr::Array<NxFr::GUID> GetDependencies() const;
		virtual void GetDependencies(NxFr::Set<NxFr::GUID>& Ids) const;

	protected:
		virtual void OnInitialize();
		virtual void OnShutdown();
		virtual void OnUpdateHierarchy();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void OnTick(float TimeStep = 0.0f);
		virtual void OnDraw();
		virtual void OnClone(const Object& Other);
		virtual void OnSerialize(NxFr::Yaml::Node& Node) const;
		virtual void OnDeserialize(const NxFr::Yaml::Node& Node);
		virtual void OnSave(NxFr::StringView Path) const;
		virtual void OnLoad(NxFr::StringView Path);
		virtual void OnUnload();
		virtual void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) const;

		NX_FORCE_INLINE bool GetFlag(ObjectFlags Flag) const;
		NX_FORCE_INLINE void SetFlag(ObjectFlags Flag, bool Value);

	private:
		ObjectFlags Flags;
	};
}
