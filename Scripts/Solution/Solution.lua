Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")
NexusFramework = os.getenv('NexusFramework') .. "/"

Name = "%{prj.name}"
Output = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"
Link = "_%{cfg.platform}_%{cfg.buildcfg}"

Framework = "NexusFramework"
Engine = "NexusEngine"
App = "NexusApp"
Editor = "NexusEditor"
AppExe = "NexusApp-Exe"
EditorExe = "NexusEditor-Exe"
SandboxApp = "NexusSandbox-App"
SandboxEditor = "NexusSandbox-Editor"

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"
Code = Sources .. Name .. "/"
External = Libraries .. Name .. "/"
Target = Binaries .. Output .. "/"
Object = Intermediates .. Output .. "/"

PostBuild = Scripts .. "Build/Steps/PostBuild.bat " .. Output

workspace (Engine)
    location (Root)

    platforms { "Win64" }
    configurations { "Debug", "Release", "Distrib" }

	startproject (StarterApp)
	debugcommand (Artifacts .. App .. ".exe")
	debugdir (Root)

	characterset "Unicode"
    flags { "MultiProcessorCompile" }

	filter "action:vs*"
        toolset "msc"

    filter "toolset:msc"
        defines { "NEXUS_MSVC" }

    filter "platforms:Win64"
        defines { "NEXUS_WINDOWS" }
        architecture "x64"
		system "windows"

    filter "configurations:Debug"
        defines { "NEXUS_DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NEXUS_RELEASE" }
        symbols "On"
        optimize "On"

    filter "configurations:Distrib"
        defines { "NEXUS_DISTRIB" }
        symbols "Off"
        optimize "On"

group "Libraries"
group "Tests"
project (SandboxApp)
project (SandboxEditor)
group "Executable"
project (AppExe)
project (EditorExe)
group "Modules"
project (App)
project (Editor)
group ""

project (Engine)
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

	pchheader ("NexusEngine/Core/NexusEnginePch.h")
	pchsource (Code .. "Core/NexusEnginePch.cpp")

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
        Code .. "**.natvis"
    }

    includedirs
    {
        Sources,
		NexusFramework .. "Sources/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. Link
	}

	links
	{
		Framework
	}

	defines
	{
		"NEXUS_ENGINE_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (App)
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
    }

    includedirs
    {
        Sources,
		NexusFramework .. "Sources/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. Link,
	}

	links
	{
		Framework,
		Engine
	}

	defines
	{
		"NEXUS_APP_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

project (Editor)
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
    }

    includedirs
    {
        Sources,
		NexusFramework .. "Sources/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. Link,
	}

	links
	{
		Framework,
		Engine,
		App
	}

	defines
	{
		"NEXUS_EDITOR_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (AppExe)
    location (Code)

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetname (App)
	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
    }

    includedirs
    {
        Sources,
    }

	libdirs
	{
	}

	links
	{
		App,
	}

	defines
	{
	}

    postbuildcommands
    {
        PostBuild
    }

project (EditorExe)
    location (Code)

    kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"

	targetname (Editor)
	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
    }

    includedirs
    {
        Sources,
    }

	libdirs
	{
	}

	links
	{
		Editor,
	}

	defines
	{
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (SandboxApp)
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
    }

    includedirs
    {
        Sources,
		NexusFramework .. "Sources/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. Link,
	}

	links
	{
		Framework,
		Engine,
		App
	}

	defines
	{
		"NEXUS_SANDBOX_APP_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }


project (SandboxEditor)
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.h",
        Code .. "**.cpp",
    }

    includedirs
    {
        Sources,
		NexusFramework .. "Sources/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. Link,
	}

	links
	{
		Framework,
		Engine,
		App,
		Editor,
		SandboxApp
	}

	defines
	{
		"NEXUS_SANDBOX_EDITOR_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }
