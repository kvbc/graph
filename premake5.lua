workspace "Graph"
    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "GraphApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    warnings "Off"

    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)

    includedirs { "include" }
    files { "**.h*", "**.c*" }
    
	libdirs { "." }
    links { "freetype", "muparser" }
	
    filter("system:not linux")
        links { "glfw3" }

	filter "system:linux"
		links { "dl", "glfw" }
