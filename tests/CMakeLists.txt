find_package(Qt6 REQUIRED COMPONENTS Test Qml Quick)

qt_add_resources(TEST_QRC ../src/resources.qrc)

add_executable(test_siriglow
    test_siriglow.cpp
    ${TEST_QRC}
)

target_link_libraries(test_siriglow PRIVATE
    ksiriglow_core
    Qt6::Test
    Qt6::Qml
    Qt6::Quick
)

add_test(NAME test_siriglow COMMAND test_siriglow)
