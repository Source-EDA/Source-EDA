# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/source_eda_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/source_eda_autogen.dir/ParseCache.txt"
  "source_eda_autogen"
  )
endif()
