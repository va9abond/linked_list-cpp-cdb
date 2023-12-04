workspace "LinkedListWorkspace"
    location "build"
    configurations { "Debug" }

project "LinkedList"
    filename ("linked_list")
    location ("build/core")
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    files {
        "xmemory.hpp",
        "msldef.h",
        "list_node.hpp",
        "list_iterator.hpp",
        "list.cpp",
        "__test.cpp"
    }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

newaction {
    trigger = "clean",
    description = "make it clean",
    execute = function ()
        os.rmdir("./build")
        os.rmdir("./bin-int")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxprog.user")
        print("--> Done <--")
    end
}
