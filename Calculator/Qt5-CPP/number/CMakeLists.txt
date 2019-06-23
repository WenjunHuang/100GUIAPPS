find_package(Qt5 REQUIRED Core Widgets)
add_library(number
        STATIC
        Number.cc
        NumberBase.h
        NumberInteger.cc
        NumberInteger.h
        NumberFloat.cc NumberFloat.h NumberError.cc NumberError.h NumberFraction.cc NumberFraction.h)
target_link_libraries(number
        PUBLIC Qt5::Core
        ${GMP_LIBRARIES}
        )

add_executable(NumberUnitTests NumberUnitTests.cc NumberUnitTests.h)
target_link_libraries(NumberUnitTests PRIVATE
        number
        GoogleTest
        pthread)
