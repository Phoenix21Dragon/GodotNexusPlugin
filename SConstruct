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

# env.Append(LIBS=[
#     'Qt5Core',
#     'Qt5Gui',
#     'Qt5Widgets'
# ])
# env.Append(LIBPATH=['/usr/lib/x86_64-linux-gnu'])
# env.Append(CPPDEFINES=['QT_CORE_LIB'])

env.ParseConfig('pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5OpenGL')


# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=[
                    "src",
                    "src/corto/include/corto",
                    "src/nexus/src/common",
                    "src/nexus/src/nxsbuild",
                    "src/nexus/src/nxszip",
                    "src/nexus/src/nxsview",
                    "src/vcglib",
                    "src/vcglib/eigenlib",
                    ])
env.Append(CXXFLAGS=["-fexceptions"])
env.Append(CXXFLAGS=['-w'])
# sources = Glob("src/*.cpp")
sources = (
    Glob("src/*.cpp") + 
    
    # From nxsview.pro
    Glob("src/vcglib/wrap/gui/trackmode.cpp") +
    Glob("src/vcglib/wrap/gui/trackball.cpp") +
    Glob("src/vcglib/wrap/system/qgetopt.cpp") +
    Glob("src/nexus/src/common/controller.cpp") +
    Glob("src/nexus/src/common/nexus.cpp") +
    Glob("src/nexus/src/common/cone.cpp") +
    Glob("src/nexus/src/common/traversal.cpp") +
    Glob("src/nexus/src/common/renderer.cpp") +
    Glob("src/nexus/src/common/ram_cache.cpp") +
    Glob("src/nexus/src/common/frustum.cpp") +
    Glob("src/nexus/src/common/nexusdata.cpp") +
    Glob("src/nexus/src/nxszip/abitstream.cpp") +
    Glob("src/nexus/src/nxszip/atunstall.cpp") +
    Glob("src/nexus/src/nxszip/meshdecoder.cpp") +
    # Glob("main.cpp") +
    # Glob("gl_nxsview.cpp") +
    Glob("src/nexus/src/nxsview/scene.cpp") +
    Glob("src/nexus/src/common/qtnexusfile.cpp") +
    Glob("src/corto/src/*.cpp") 
)

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
