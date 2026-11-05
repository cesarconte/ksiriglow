add_library(ksiriglow_core STATIC
    siriglowcontroller.cpp
    siriglowcontroller.h
)

target_include_directories(ksiriglow_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ksiriglow_core PUBLIC
    Qt6::Core
    Qt6::DBus
    Qt6::Gui
    KF6::CoreAddons
    KF6::I18n
)

qt_add_resources(KSIRIGLOW_QRC resources.qrc)

add_executable(ksiriglow
    main.cpp
    ${KSIRIGLOW_QRC}
)

target_link_libraries(ksiriglow PRIVATE
    ksiriglow_core
    Qt6::Quick
    Qt6::Qml
    KF6::I18n
)

install(TARGETS ksiriglow ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
