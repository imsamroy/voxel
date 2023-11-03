add_requires("glad", "glfw", "glm", "stb")
set_languages("c++20")
set_optimize("fastest")

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
  add_cflags("-g", "-w")
  add_linkdirs("lib")
  add_links("FastNoise")
  add_deps("imgui", "stb")