#pragma once

#include "NexusEngine/Core/NexusEngineCore.h"
#include "NexusEngine/Application/Object/Flags.h"

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
	class NX_ENGINE_API Object
	{
	public:
		NX_OBJECT(Object)

		Object();
		virtual ~Object();

		bool operator==(const Object& Other) const;
		bool operator!=(const Object& Other) const;

		virtual void Initialize();
		virtual void Shutdown();
		virtual void Tick(float TimeStep = 0.0f);
		virtual void Draw();

		virtual Object* Clone() const;
		virtual void Clone(Object* Other) const;
		virtual void Clone(const Object* Other);
		virtual YAML::Node Save();
		virtual void Load(const YAML::Node& Node);
		virtual void Unload();
		virtual NxFr::Array<NxFr::GUID> GetDependencies();

		virtual NxFr::GUID GetId() const;
		virtual NxFr::StringView GetName() const;
		virtual bool IsInitialized() const;
		virtual bool IsEnabled() const;
		virtual void SetEnabled(bool Enabled);
		virtual bool IsTickable() const;
		virtual void SetTickable(bool Tickable);
		virtual bool IsTicking() const;

	protected:
		virtual void OnInitialize() {};
		virtual void OnShutdown() {};
		virtual void OnEnable() {};
		virtual void OnDisable() {};
		virtual void OnTick(float TimeStep = 0.0f) {};
		virtual void OnDraw() {};

		virtual void OnClone(const Object& Other) {};
		virtual void OnSave(YAML::Node& Node) {};
		virtual void OnLoad(const YAML::Node& Node) {};
		virtual void OnUnload() {};
		virtual void OnGetDependencies(NxFr::Set<NxFr::GUID>& Ids) {};

		NX_FORCE_INLINE bool GetFlag(ObjectFlags Flag) const;
		NX_FORCE_INLINE void SetFlag(ObjectFlags Flag, bool Value);

	private:
		ObjectFlags Flags;
	};
}
