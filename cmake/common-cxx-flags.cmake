# Exported variables:
# - COMMON_CXX_FLAGS

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  list(APPEND COMMON_CXX_FLAGS
      -std=c++11 -Wall -Werror -Wextra)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  list(APPEND COMMON_CXX_FLAGS
      -std=c++11 -Wall -Werror -Wextra)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  list(APPEND COMMON_CXX_FLAGS
      /W4)
else()
  message(FATAL_ERROR "ERROR! Use 'Clang', 'GNU' or 'MSVC'.")
endif()
