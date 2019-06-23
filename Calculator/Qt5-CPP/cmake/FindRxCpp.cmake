#if (NOT EXISTS "${rxcpp_INCLUDE_DIR}")
#    find_path(rxcpp_INCLUDE_DIR
#            NAMES rxcpp/rx.hpp
#            DOC "rxcpp library header files"
#            )
#endif ()

include(ExternalProject)
ExternalProject_Add(rxcpp
        URL https://github.com/ReactiveX/RxCpp/archive/v4.1.0.zip
        INSTALL_COMMAND ""
        BUILD_COMMAND ""
        UPDATE_COMMAND "")

ExternalProject_Get_Property(rxcpp source_dir)
set(rxcpp_INCLUDE_DIR ${source_dir}/Rx/v2/src)
add_library(RxCpp INTERFACE)
add_dependencies(RxCpp rxcpp)
target_include_directories(RxCpp INTERFACE ${rxcpp_INCLUDE_DIR})

if (EXISTS "${rxcpp_INCLUDE_DIR}")
    set(RxCpp_FOUND 1)
else ()
    set(RxCpp_FOUND 0)
endif ()
