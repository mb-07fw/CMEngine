# Common CMakeLists.txt

# NOTE: Everything wrote in Common is meant to be local to a specific dll-boundary,
#         and thus not shared across them.

set(Build_Files 
    "Dummy.cpp" # to ensure there is a .lib to link to...
)

add_library(Common STATIC ${Build_Files})

set(Output_Dir "${CMAKE_BINARY_DIR}/Common/out")

target_include_directories(Common PUBLIC "${CMAKE_SOURCE_DIR}")

SetCommon(Common ${Output_Dir})

target_compile_features(Common PUBLIC cxx_std_20)