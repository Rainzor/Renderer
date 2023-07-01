# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Renderer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Renderer_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\Test_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Test_autogen.dir\\ParseCache.txt"
  "Renderer_autogen"
  "Test_autogen"
  )
endif()
