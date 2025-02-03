Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")
NexusFramework = os.getenv('NexusFramework') .. "/"

Name = "%{prj.name}"
Output = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"
Link = "_%{cfg.platform}_%{cfg.buildcfg}"

Framework = "NexusFramework"
Engine = "NexusEngine"
App = "NexusApp"

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

	startproject (App)
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
group "Modules"
project (App)
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

project (App)
    location (Code)

    kind "ConsoleApp"
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
		Engine
	}

	links
	{
		Framework,
		Engine
	}

	defines
	{
	}

    postbuildcommands
    {
        PostBuild
    }
