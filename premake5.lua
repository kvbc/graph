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
    links { "glfw", "freetype", "muparser" }
	
	filter "system:linux"
		links { "dl" }
