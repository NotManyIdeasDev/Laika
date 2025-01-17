workspace "Laika"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "LaikaCLI"

    outDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"
    
    project "LaikaLib"
        location "Build/Core"
        kind "SharedLib"
        language "C"

        targetdir   ( "Binaries/" .. outDir .. "/%{prj.name}" )
        objdir      ( "Binaries/Objects/" .. outDir .. "/%{prj.name}" )

        files { "Build/Core/Source/**.c", "Build/Core/Include/**.h" }
        includedirs { "Build/Core/Include" }

        defines { "LKA_BUILD_DLL" }

        filter "system:Windows"
            staticruntime "On"
            systemversion "latest"
            system "windows"
            defines { "LKA_WIN", "_CRT_SECURE_NO_WARNINGS" }

            postbuildcommands
            {
                "{COPY} ../../Binaries/" .. outDir .. "/LaikaLib/*.dll ../../Binaries/" .. outDir .. "/LaikaCLI",
                "{COPY} ../../Binaries/" .. outDir .. "/LaikaLib/*.lib ../../Binaries/" .. outDir .. "/LaikaCLI"
            }

        filter "system:Macosx"
            system "macosx"
            defines { "LKA_MACOS" }

            prelinkcommands
            {
               ("mkdir -p ../../Binaries/" .. outDir .. "/LaikaCLI")
            }
    
            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../../Binaries/" .. outDir .. "/LaikaCLI")
            }

        filter "system:Linux"
            pic "On"
            system "Linux"
            defines { "LKA_LINUX" }
            buildoptions { "-Wno-unused-result" }

            prelinkcommands
            {
               ("mkdir -p ../../Binaries/" .. outDir .. "/LaikaCLI")
            }

            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../../Binaries/" .. outDir .. "/LaikaCLI")
            }

        filter { "configurations:Debug" }
            defines { "LKA_DEBUG", "DEBUG" }
            optimize "Debug"
            symbols "On"

        filter { "configurations:Release" }
            defines { "LKA_RELEASE", "NDEBUG" }
            optimize "Full"
            symbols "Off"

    project "LaikaCLI"
        location "Build/CLI"
        kind "ConsoleApp"
        language "C"

        targetdir   ( "Binaries/" .. outDir .. "/%{prj.name}" )
        objdir      ( "Binaries/Objects/" .. outDir .. "/%{prj.name}" )

        files { "Build/CLI/Source/**.c", "Build/CLI/Include/**.h" }
        includedirs { "Build/CLI/Include", "Build/Core/Include" }

        links { "LaikaLib" }

        filter "system:Windows"
            staticruntime "On"
            systemversion "latest"
            system "windows"
            defines { "CLI_WIN", "_CRT_SECURE_NO_WARNINGS" }

        filter "system:Macosx"
            system "macosx"
            defines { "CLI_MACOS" }

        filter "system:Linux"
            pic "On"
            system "Linux"
            defines { "CLI_LINUX" }
            buildoptions { "-Wno-unused-result" }

        filter { "configurations:Debug" }
            defines { "CLI_DEBUG", "DEBUG" }
            optimize "Debug"
            symbols "On"

        filter { "configurations:Release" }
            defines { "CLI_RELEASE", "NDEBUG" }
            optimize "Full"
            symbols "Off"