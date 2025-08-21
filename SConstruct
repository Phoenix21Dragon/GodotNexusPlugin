#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=[
                    "src/corto/include/corto",
                    "src",
                    "src/nexus/src/common",
                    "src/nexus/src/nxsbuild",
                    "src/nexus/src/nxszip",
                    "src/vcglib",
                    "src/vcglib/eigenlib",
                    "src/corto/include",
                    # "/usr/include/x86_64-linux-gnu/qt5/QtCore",
                    # "/usr/include/x86_64-linux-gnu/qt5"
                    ])
env.Append(CXXFLAGS=["-fexceptions"])
# sources = Glob("src/*.cpp")
sources = (Glob("src/*.cpp") + 
           Glob("src/nexus/src/common/nexus.cpp",) + 
           Glob("src/nexus/src/common/nexusdata.cpp",) +
           Glob("src/nexus/src/common/qtnexusfile.cpp",) +
        #    Glob("src/nexus/src/common/controller.cpp",) +
        #    Glob("src/nexus/src/nxsbuild/*.cpp") +
        #    Glob("src/nexus/src/nxsedit/*.cpp") +
        #    Glob("src/nexus/src/nxsview/*.cpp") +
        #    Glob("src/nexus/src/nxszip/*.cpp") +
           Glob("src/vcglib/*.cpp") + 
           Glob("src/corto/src/*.cpp"))


if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libgdexample.{}.{}.framework/libgdexample.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "demo/bin/libgdexample.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "demo/bin/libgdexample.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "demo/bin/libgdexample{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
