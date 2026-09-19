Framework = "NexusFramework"
Engine = "NexusEngine"
App = "NexusApp"
Editor = "NexusEditor"
Starter = "NexusStarter"
Utility = "NexusUtility"
Project = "NexusProject"
Sandbox = "NexusSandbox"
GTest = "GTest"
YamlCpp = "yaml-cpp"
Glfw = "Glfw"
ImGui = "ImGui"
StbImage = "StbImage"
ModeApp = "-App"
ModeEditor = "-Editor"

ProjectName = "%{prj.name}"
OutputDirectory = "%{prj.name}_%{cfg.platform}_%{cfg.buildcfg}"
OutputName = "%{cfg.buildtarget.basename}%{cfg.buildtarget.extension}"
LinkConfig = "_%{cfg.platform:gsub('-Editor', '')}_%{cfg.buildcfg}"
Language = "C++"
LanguageVersion = "C++20"
Systems = 
{
    Windows = "windows"
}
Platforms = 
{
    Win64 = "Win64",
	Win64Editor = "Win64-Editor"
}
Configurations = 
{
    Debug  = "Debug",
    Release = "Release",
    Distrib = "Distrib"
}
Compilers = 
{
    Msvc = "msc"
}
Tools = 
{
    VisualStudio = "vs*"
}
Warnings = 
{
    "4244", "4251", "4267", "4275"
}

Root = os.realpath(os.getcwd() .. "/../../"):gsub("\\", "/")
InstallFramework = os.getenv(Framework) .. "/"

Builds = Root .. "builds/"
Configs = Root .. "Configs/"
Libraries = Root .. "Libraries/"
Scripts = Root .. "Scripts/"
Sources = Root .. "Sources/"

Artifacts = Builds .. "artifacts/"
Binaries = Builds .. "binaries/"
Intermediates = Builds .. "intermediates/"
FirstParty = Sources .. ProjectName .. "/"
ThirdParty = Libraries .. ProjectName .. "/"
Target = Binaries .. OutputDirectory .. "/"
Object = Intermediates .. OutputDirectory .. "/"

Includes = 
{
	Sources,
	Libraries,
	InstallFramework .. "Sources/",
	InstallFramework .. "Libraries/"
}

LibrariesIncludes = 
{
	InstallFramework .. "Builds/" .. Framework .. LinkConfig,
	InstallFramework .. "Builds/" .. GTest .. LinkConfig,
	InstallFramework .. "Builds/" .. YamlCpp .. LinkConfig
}

Links = 
{
	Framework,
	YamlCpp,
	GTest,
	ImGui
}

Defines = 
{
	"GTEST_LINKED_AS_SHARED_LIBRARY",
	'IMGUI_USER_CONFIG="NexusEngine/External/ImGui/Config.h"'
}

PostBuild = Scripts .. "Build/Steps/PostBuild.bat " .. OutputDirectory .. " " .. OutputName

-- ----------------------------------------------------------------------------------
workspace (Engine)
    location (Root)

    platforms { Platforms.Win64, Platforms.Win64Editor }
    configurations { Configurations.Debug, Configurations.Release, Configurations.Distrib }

	startproject (Starter)
	debugcommand (Artifacts .. Starter .. ".exe")
	debugdir (Root)
	filter ("platforms:" .. Platforms.Win64)
		debugargs { "-Mode=App" }
	filter ("platforms:" .. Platforms.Win64Editor)
		debugargs { "-Mode=Editor" }
	filter ""

	characterset "Unicode"
    flags { "MultiProcessorCompile" }
    staticruntime "off"

	filter ("action:" .. Tools.VisualStudio)
        toolset (Compilers.Msvc)

    filter ("toolset:" .. Compilers.Msvc)
        defines { "NX_MSVC", "_CRT_SECURE_NO_WARNINGS" }
    	disablewarnings (Warnings)

    filter ("platforms:" .. Platforms.Win64 .. "*")
        defines { "NX_WINDOWS" }
		system (Systems.Windows)
        architecture "x64"

	filter ("platforms:*" .. ModeEditor)
        defines { "NX_EDITOR" }

    filter ("configurations:" .. Configurations.Debug)
        defines { "NX_DEBUG" }
        symbols "On"
        optimize "Off"

    filter ("configurations:" .. Configurations.Release)
        defines { "NX_RELEASE" }
        symbols "On"
        optimize "On"

    filter ("configurations:" .. Configurations.Distrib)
        defines { "NX_DISTRIB" }
        symbols "Off"
        optimize "On"

    filter ""

group "Libraries"
project (Glfw)
project (ImGui)
project (StbImage)
group "Tests"
project (Sandbox .. ModeApp)
project (Sandbox .. ModeEditor)
group "Misc"
project (Utility)
group "Modules"
project (App)
project (Editor)
project (Starter)
group ""

-- ----------------------------------------------------------------------------------
project (Engine)
    location (FirstParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

	pchheader ("NexusEngine/Core/NexusEnginePch.h")
	pchsource (FirstParty .. "Core/NexusEnginePch.cpp")

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp",
    }

    includedirs
    {
        Includes
    }

	libdirs
	{
		LibrariesIncludes
	}

	links
	{
		Links,
		Glfw,
		StbImage
	}

	defines
	{
		Defines,
		"NX_ENGINE_DLL",
		"GLFW_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (App)
    location (FirstParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp",
    }

    includedirs
    {
        Includes
    }

	libdirs
	{
		LibrariesIncludes
	}

	links
	{
		Links,
		Engine
	}

	defines
	{
		Defines,
		"NX_APP_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

project (Editor)
    location (FirstParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp",
    }

    includedirs
    {
        Includes
    }

	libdirs
	{
		LibrariesIncludes
	}

	links
	{
		Links,
		Engine,
		App
	}

	defines
	{
		Defines,
		"NX_EDITOR_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }

project (Starter)
    location (FirstParty)

    kind "ConsoleApp"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp",
    }

    includedirs
    {
        Includes
    }

	libdirs
	{
		LibrariesIncludes
	}

	links
	{
		Links,
		Engine
	}

	defines
	{
		Defines
	}

    postbuildcommands
    {
        PostBuild
    }

-- ----------------------------------------------------------------------------------
project (Utility)
    location (FirstParty)

    kind "Utility"

    targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.natvis",
		InstallFramework .. "Sources/" .. "**.natvis",
    }

-- ----------------------------------------------------------------------------------
project (Sandbox .. ModeApp)
    location (FirstParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetname (Project .. ModeApp)
	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp",
    }

    includedirs
    {
        Includes
    }

	libdirs
	{
		LibrariesIncludes
	}

	links
	{
		Links,
		Engine,
		App,
	}

	defines
	{
		Defines,
		"NX_SANDBOX_APP_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }


project (Sandbox .. ModeEditor)
    location (FirstParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetname (Project .. ModeEditor)
	targetdir (Target)
	objdir (Object)

    files
    {
        FirstParty .. "**.h",
        FirstParty .. "**.cpp",
    }

    includedirs
    {
        Includes
    }

	libdirs
	{
		LibrariesIncludes
	}

	links
	{
		Links,
		Engine,
		App,
		Editor,
		Sandbox .. ModeApp,
	}

	defines
	{
		Defines,
		"NX_SANDBOX_EDITOR_DLL"
	}

    postbuildcommands
    {
        PostBuild
    }


-- ----------------------------------------------------------------------------------
project (Glfw)
    location (ThirdParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

	files
    {
        ThirdParty .. "include/GLFW/glfw3.h",
		ThirdParty .. "include/GLFW/glfw3native.h",

		ThirdParty .. "src/context.c",
		ThirdParty .. "src/init.c",
		ThirdParty .. "src/input.c",
		ThirdParty .. "src/internal.h",
		ThirdParty .. "src/monitor.c",
		ThirdParty .. "src/platform.c",
		ThirdParty .. "src/vulkan.c",
		ThirdParty .. "src/window.c",

		ThirdParty .. "src/null_init.c",
		ThirdParty .. "src/null_joystick.c",
		ThirdParty .. "src/null_monitor.c",
		ThirdParty .. "src/null_platform.h",
		ThirdParty .. "src/null_window.c",
    }

	includedirs
    {
        Libraries
    }

	defines
	{
		"_GLFW_BUILD_DLL"
	}

	postbuildcommands
    {
        PostBuild
    }

	filter ("system:" .. Systems.Windows)
		files
		{
			ThirdParty .. "src/win32_init.c",
			ThirdParty .. "src/win32_joystick.c",
			ThirdParty .. "src/win32_joystick.h",
			ThirdParty .. "src/win32_module.c",
			ThirdParty .. "src/win32_monitor.c",
			ThirdParty .. "src/win32_platform.h",
			ThirdParty .. "src/win32_time.c",
			ThirdParty .. "src/win32_time.h",
			ThirdParty .. "src/win32_thread.c",
			ThirdParty .. "src/win32_thread.h",
			ThirdParty .. "src/win32_window.c",
			ThirdParty .. "src/wgl_context.c",
			ThirdParty .. "src/egl_context.c",
			ThirdParty .. "src/osmesa_context.c",
		}

		defines
		{
			"_GLFW_WIN32",
		}

project (ImGui)
    location (ThirdParty)

    kind "SharedLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

	files
    {
        ThirdParty .. "**.h",
		ThirdParty .. "**.cpp",
		ThirdParty .. "**.natvis",
    }

	includedirs
	{
		Libraries,
		Libraries .. "glfw/include/"
	}

	links
	{
		Glfw
	}

	defines
	{
		'IMGUI_USER_CONFIG="' .. Sources .. 'NexusEngine/External/ImGui/Config.h"',
		"IMGUI_DLL",
		"GLFW_DLL"
	}

	postbuildcommands
    {
        PostBuild
    }

project (StbImage)
    location (ThirdParty)

    kind "StaticLib"
    language (Language)
	cppdialect (LanguageVersion)

	targetdir (Target)
	objdir (Object)

	files
    {
        ThirdParty .. "**.h",
		ThirdParty .. "**.cpp",
    }

	includedirs
    {
        Libraries
    }

	postbuildcommands
    {
        PostBuild
    }