include(ExternalProject)
ExternalProject_Add(gtest
        URL https://github.com/google/googletest/archive/release-1.8.1.zip
        INSTALL_COMMAND ""
        )

ExternalProject_Get_Property(gtest binary_dir)
ExternalProject_Get_Property(gtest source_dir)
message("${binary_dir}")
message("${source_dir}")
add_library(GoogleTest INTERFACE)
target_include_directories(GoogleTest INTERFACE ${source_dir}/googletest/include
        ${source_dir}/googlemock/include)
target_link_libraries(GoogleTest INTERFACE ${binary_dir}/googlemock/gtest/libgtest.a
        ${binary_dir}/googlemock/gtest/libgtest_main.a
        ${binary_dir}/googlemock/libgmock.a)
#set_target_properties(GoogleTest PROPERTIES IMPORTED_LOCATION ${binary_dir}/libgtest.a)

add_dependencies(GoogleTest gtest)

set(GoogleTest_FOUND 1)
