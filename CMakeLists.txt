cmake_minimum_required(VERSION 3.19)
project(GroupProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)

find_package(VTK REQUIRED COMPONENTS
  RenderingOpenVR
  CommonCore
  FiltersSources
  InteractionStyle
  RenderingCore        
  GUISupportQt
  IOGeometry
)

qt_standard_project_setup()
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)
qt_add_executable(GroupProject
  WIN32 MACOSX_BUNDLE

  src/main.cpp
  src/mainwindow.cpp
  src/ModelPart.cpp
  src/ModelPartList.cpp
  src/optiondialog.cpp
  src/VRManager.cpp

  include/mainwindow.h
  include/ModelPart.h
  include/ModelPartList.h
  include/optiondialog.h
  include/VRManager.h

  ui/mainwindow.ui
  ui/optiondialog.ui

)

target_include_directories(GroupProject PRIVATE
  include
  ui
)

target_link_libraries(GroupProject PRIVATE
  Qt::Core
  Qt::Widgets
  Qt::OpenGLWidgets
  VTK::CommonCore
  VTK::FiltersSources
  VTK::InteractionStyle
  VTK::RenderingCore
  VTK::GUISupportQt
  VTK::IOGeometry
  VTK::RenderingOpenVR
)

vtk_module_autoinit(
  TARGETS GroupProject
  MODULES
    VTK::RenderingOpenVR
    VTK::CommonCore
    VTK::FiltersSources
    VTK::InteractionStyle
    VTK::RenderingCore
    VTK::GUISupportQt
    VTK::IOGeometry
)

add_custom_command(TARGET GroupProject PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/vrbindings
          ${CMAKE_BINARY_DIR}/vrbindings
)
install(TARGETS GroupProject
  RUNTIME DESTINATION bin
)

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "GroupProject")
set(CPACK_PACKAGE_VENDOR "EEEE2076")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EEEE2076 Group VR Project")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_NSIS_DISPLAY_NAME "GroupProject")
set(CPACK_NSIS_PACKAGE_NAME "GroupProject")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

# ---- Deploy Qt DLL automatically ----
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS
    "${Qt6_DIR}/../../../bin"
)

add_custom_command(TARGET GroupProject POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE}
        --debug
        --no-translations
        $<TARGET_FILE:GroupProject>
)

install(CODE "
    execute_process(
        COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
        --release
        --no-translations
        --dir \"\${CMAKE_INSTALL_PREFIX}/bin\"
        \"\${CMAKE_INSTALL_PREFIX}/bin/GroupProject.exe\"
    )
")

include(CPack)