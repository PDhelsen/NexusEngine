Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")
NexusFramework = os.getenv('NexusFramework') .. "/"

Name = "%{prj.name}"
OutputDirectory = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"
OutputName = "%{cfg.buildtarget.basename}%{cfg.buildtarget.extension}"
LinkFramework = "_%{cfg.platform:gsub('-Editor', '')}_%{cfg.buildcfg}"

Framework = "NexusFramework"
Engine = "NexusEngine"
App = "NexusApp"
Editor = "NexusEditor"
Starter = "NexusStarter"
Utility = "NexusUtility"
Project = "NexusProject"
Sandbox = "NexusSandbox"
Yaml = "yaml-cpp"
Glfw = "Glfw"
ImGui = "ImGui"

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"
Code = Sources .. Name .. "/"
Lib = Libraries .. Name .. "/"
Target = Binaries .. OutputDirectory .. "/"
Object = Intermediates .. OutputDirectory .. "/"

PostBuild = Scripts .. "Build/Steps/PostBuild.bat " .. OutputDirectory .. " " .. OutputName

workspace (Engine)
    location (Root)

    platforms { "Win64", "Win64-Editor" }
    configurations { "Debug", "Release", "Distrib" }

	startproject (Starter)
	debugcommand (Artifacts .. Starter .. ".exe")
	debugdir (Root)
	filter "platforms:Win64"
		debugargs { "-Mode=App" }
	filter "platforms:Win64-Editor"
		debugargs { "-Mode=Editor" }
	filter {}

	characterset "Unicode"
    flags { "MultiProcessorCompile" }

	filter "action:vs*"
        toolset "msc"

    filter "toolset:msc"
        defines { "NEXUS_MSVC" }
    	disablewarnings { "4244", "4267" }

    filter "platforms:Win64*"
        defines { "NEXUS_WINDOWS" }
        architecture "x64"
		system "windows"

    filter "platforms:*-Editor"
        defines { "NEXUS_EDITOR" }

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
project (Glfw)
project (ImGui)
group "Tests"
project (Sandbox .. "-App")
project (Sandbox .. "-Editor")
group "Misc"
project (Utility)
group "Modules"
project (App)
project (Editor)
project (Starter)
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
    }

    includedirs
    {
        Sources,
		Libraries,
		NexusFramework .. "Sources/",
		NexusFramework .. "Libraries/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. LinkFramework,
		NexusFramework .. "Builds/yaml-cpp" .. LinkFramework
	}

	links
	{
		Framework,
		Yaml,
		Glfw,
		ImGui
	}

	defines
	{
		"NEXUS_ENGINE_DLL",
		"GLFW_DLL"
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
		Libraries,
		NexusFramework .. "Sources/",
		NexusFramework .. "Libraries/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. LinkFramework,
		NexusFramework .. "Builds/yaml-cpp" .. LinkFramework
	}

	links
	{
		Framework,
		Yaml,
		Engine,
		ImGui
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
		Libraries,
		NexusFramework .. "Sources/",
		NexusFramework .. "Libraries/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. LinkFramework,
		NexusFramework .. "Builds/yaml-cpp" .. LinkFramework
	}

	links
	{
		Framework,
		Yaml,
		Engine,
		App,
		ImGui
	}

	defines
	{
		"NEXUS_EDITOR_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

project (Starter)
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
		Libraries,
		NexusFramework .. "Sources/",
		NexusFramework .. "Libraries/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. LinkFramework
	}

	links
	{
		Framework,
		Engine,
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (Utility)
    location (Code)

    kind "Utility"

    targetdir (Target)
	objdir (Object)

    files
    {
        Code .. "**.natvis",
		NexusFramework .. "Sources/" .. "**.natvis",
    }

-- ----------------------------------------------------------------------------------
project (Sandbox .. "-App")
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetname (Project .. "-App")
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
		Libraries,
		NexusFramework .. "Sources/",
		NexusFramework .. "Libraries/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. LinkFramework,
		NexusFramework .. "Builds/yaml-cpp" .. LinkFramework
	}

	links
	{
		Framework,
		Yaml,
		Engine,
		App,
		ImGui
	}

	defines
	{
		"NEXUS_SANDBOX_APP_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }


project (Sandbox .. "-Editor")
    location (Code)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetname (Project .. "-Editor")
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
		Libraries,
		NexusFramework .. "Sources/",
		NexusFramework .. "Libraries/"
    }

	libdirs
	{
		NexusFramework .. "Builds/NexusFramework" .. LinkFramework,
		NexusFramework .. "Builds/yaml-cpp" .. LinkFramework
	}

	links
	{
		Framework,
		Yaml,
		Engine,
		App,
		Editor,
		Sandbox .. "-App",
		ImGui
	}

	defines
	{
		"NEXUS_SANDBOX_EDITOR_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }


-- ----------------------------------------------------------------------------------
project (Glfw)
    location (Lib)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

	files
    {
        Lib .. "include/GLFW/glfw3.h",
		Lib .. "include/GLFW/glfw3native.h",

		Lib .. "src/context.c",
		Lib .. "src/init.c",
		Lib .. "src/input.c",
		Lib .. "src/internal.h",
		Lib .. "src/monitor.c",
		Lib .. "src/platform.c",
		Lib .. "src/vulkan.c",
		Lib .. "src/window.c",

		Lib .. "src/null_init.c",
		Lib .. "src/null_joystick.c",
		Lib .. "src/null_monitor.c",
		Lib .. "src/null_platform.h",
		Lib .. "src/null_window.c",
    }

	defines
	{
		"_GLFW_BUILD_DLL"
	}

	postbuildcommands
    {
        PostBuild
    }

	filter "system:windows"
		files
		{
			Lib .. "src/win32_init.c",
			Lib .. "src/win32_joystick.c",
			Lib .. "src/win32_joystick.h",
			Lib .. "src/win32_module.c",
			Lib .. "src/win32_monitor.c",
			Lib .. "src/win32_platform.h",
			Lib .. "src/win32_time.c",
			Lib .. "src/win32_time.h",
			Lib .. "src/win32_thread.c",
			Lib .. "src/win32_thread.h",
			Lib .. "src/win32_window.c",
			Lib .. "src/wgl_context.c",
			Lib .. "src/egl_context.c",
			Lib .. "src/osmesa_context.c",
		}

		defines
		{
			"_GLFW_WIN32",
		}

	filter "toolset:msc"
		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

project (ImGui)
    location (Lib)

    kind "SharedLib"
    language "C++"
	cppdialect "C++20"

	targetdir (Target)
	objdir (Object)

	files
    {
        Lib .. "**.h",
		Lib .. "**.cpp",
		Lib .. "**.natvis",
    }

	includedirs
	{
		Libraries
	}

	links
	{
		Glfw
	}

	defines
	{
		"IMGUI_DLL",
		"GLFW_DLL"
	}

	postbuildcommands
    {
        PostBuild
    }
