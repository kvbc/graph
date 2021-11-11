workspace "Graph"
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "GRAPH_DEBUG" }
        -- symbols "on"

    filter "configurations:Release"
        optimize "on"

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "GraphApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    warnings "off"

    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)

    includedirs { "include" }
    files { "**.h*", "**.c*" }
    
	libdirs { "." }
    links { "freetype", "muparser" }
	
    filter "system:not linux"
        links { "glfw3" }

	filter "system:linux"
		links { "dl", "glfw" }
