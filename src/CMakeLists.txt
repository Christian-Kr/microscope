if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/rsrc/mainresources.qrc
    main.cpp
    mainwin.cpp
    controller.cpp
    livecamera.cpp
    stitchingwidget.cpp
    imagepreview.cpp
    autostitchingstatus.cpp
)

target_link_libraries(${PROJECT_NAME} 
    Qt5::Widgets 
    Qt5::SerialPort 
    ${OpenCV_LIBS}
)
