find_package(PkgConfig)

if(NOT EXISTS "${rxcpp_INCLUDE_DIR}")
    find_path(rxcpp_INCLUDE_DIR
            NAMES rxcpp/rx.hpp
            DOC "rxcpp library header files"
            )
endif()

if(EXISTS "${rxcpp_INCLUDE_DIR}")
    include(FindPackageHandleStandardArgs)
    mark_as_advanced(rxcpp_INCLUDE_DIR)
else()
    include(ExternalProject)
    ExternalProject_Add(rxcpp
            GIT_REPOSITORY https://github.com/ReactiveX/RxCpp.git
            GIT_TAG master
            GIT_SHALLOW 1
            CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
            PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
            CONFIGURE_COMMAND "" # Disable configure step
            BUILD_COMMAND "" # Disable build step
            INSTALL_COMMAND "" # Disable install step
            UPDATE_COMMAND "" # Disable update step: clones the project only once
            )
    ExternalProject_Get_Property(rxcpp source_dir)
    set(rxcpp_INCLUDE_DIR ${source_dir}/Rx/v2/src)
#    add_library(RxCpp INTERFACE)
#    target_include_directories(RxCpp INTERFACE ${rxcpp_INCLUDE_DIR})

    # Specify include dir
endif()

if(EXISTS "${rxcpp_INCLUDE_DIR}")
    set(rxcpp_FOUND 1)
else()
    set(rxcpp_FOUND 0)
endif()
