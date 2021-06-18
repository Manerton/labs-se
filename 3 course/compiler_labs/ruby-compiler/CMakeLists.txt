# -*- mode:cmake -*-
project(ruby-compiler LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT UNIX)
  message(WARNING "Unsupported operating system")
endif()

set(ruby-compiler-GENERATED_SRC
   ${PROJECT_SOURCE_DIR}/generated/TLexer.cpp
   ${PROJECT_SOURCE_DIR}/generated/TParser.cpp
   ${PROJECT_SOURCE_DIR}/generated/TParserBaseVisitor.cpp
   ${PROJECT_SOURCE_DIR}/generated/TParserVisitor.cpp
 )

foreach(src_file ${ruby-compiler-GENERATED_SRC})
      set_source_files_properties(
          ${src_file}
          PROPERTIES
          GENERATED TRUE
          )
endforeach(src_file ${ruby-compiler-GENERATED_SRC})

include_directories(
  ${PROJECT_SOURCE_DIR}/runtime/src
  ${PROJECT_SOURCE_DIR}/runtime/src/misc
  ${PROJECT_SOURCE_DIR}/runtime/src/atn
  ${PROJECT_SOURCE_DIR}/runtime/src/dfa
  ${PROJECT_SOURCE_DIR}/runtime/src/tree
  ${PROJECT_SOURCE_DIR}/runtime/src/support
  ${PROJECT_SOURCE_DIR}/generated
  )

set(ruby-compiler_SRC
  src/main.cpp
  src/CodeGenerator.h src/CodeGenerator.cpp
  )

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set (flags_1 "-Wno-overloaded-virtual")
else()
  set (flags_1 "-MP /wd4251")
endif()

foreach(src_file ${ruby-compiler_SRC})
      set_source_files_properties(
          ${src_file}
          PROPERTIES
          COMPILE_FLAGS "${COMPILE_FLAGS} ${flags_1}"
          )
endforeach(src_file ${ruby-compiler_SRC})

add_executable(ruby-compiler
  ${ruby-compiler_SRC}
  ${ruby-compiler-GENERATED_SRC}
  )
#add_precompiled_header(ruby-compiler ${PROJECT_SOURCE_DIR}/runtime/src/antlrcpp-Prefix.h)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(ruby-compiler PRIVATE "/MT$<$<CONFIG:Debug>:d>")
endif()

target_link_libraries(ruby-compiler ${PROJECT_SOURCE_DIR}/runtime/lib/libantlr4-runtime.dll.a)
