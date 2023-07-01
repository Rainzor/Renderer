# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Example_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Example_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\Renderer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Renderer_autogen.dir\\ParseCache.txt"
  "Example_autogen"
  "Renderer_autogen"
  )
endif()
