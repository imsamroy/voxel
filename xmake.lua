add_requires("glad", "glfw", "glm", "stb")
set_languages("c++20")

target("imgui")
  set_kind("static")
  add_files("lib/imgui/*.cpp")
  add_includedirs("include/imgui")

target("stb")
  set_kind("static")
  add_files("lib/stb/*.cpp")
  add_packages("stb")

target("voxel")
  set_kind("binary")
  add_files("src/*.cpp")
  add_includedirs("include", "shaders")
  add_packages("glad", "glfw", "glm", "stb")
  add_cflags("-g", "-w", "-msse2")
  add_linkdirs("lib")
  add_syslinks("glfw", "GLU", "GL", "dl", "pthread")
  add_links("FastNoise")
  add_deps("imgui", "stb")
