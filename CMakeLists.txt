cmake_minimum_required(VERSION 3.16)

project(source_eda VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)

set(PROJECT_SOURCES
        main.cpp
        sourceeda.cpp sourceeda.hpp
        librarymanager.hpp librarymanager.cpp
        notification.hpp notification.cpp
        notificationpopup.hpp notificationpopup.cpp
        log.hpp log.cpp
        popup.hpp popup.cpp
        ui/mumwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(source_eda
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        sourceeda.qrc
        ui/create_lib_popup.ui
        ext_libs/json.hpp
        ui/create_cell_popup.ui
        ui/create_cellview_popup.ui
        ui/notificationpopup.ui
    )

qt_add_translations(source_eda TS_FILES source_eda_en_GB.ts)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET source_eda APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(source_eda SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(source_eda
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(source_eda PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.source_eda)
endif()
set_target_properties(source_eda PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "Source EDA"
)


include(GNUInstallDirs)
install(TARGETS source_eda
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(source_eda)
endif()
