cmake_minimum_required(VERSION 3.16)

set(JSON_FILE_PATH "${CMAKE_SOURCE_DIR}/PropertySetting/Program_Property.json")
file(READ ${JSON_FILE_PATH} JSON_CONTENTS)
set(TARGET_KEY "targetName")
string(REGEX MATCH "\"${TARGET_KEY}\": *\"([^\"]+)\"" _ "${JSON_CONTENTS}")
if(CMAKE_MATCH_1)
    set(EXTRACTED_VALUE "${CMAKE_MATCH_1}")
    message(STATUS "Success: ${EXTRACTED_VALUE}")
else()
    message(FATAL_ERROR "Error: ${TARGET_KEY}")
endif()


project(${EXTRACTED_VALUE} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # 不使用编译器特定的扩展

# 查找Qt组件
find_package(Qt6 COMPONENTS Core Gui Network Widgets LinguistTools REQUIRED)
if(Qt6_FOUND)
    set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Network Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Core Gui Network Widgets REQUIRED)
    set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)
endif()

# 自动处理moc、uic和rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 源文件和头文件
set(SRCS
    Sources/SystemKit/BaseScrollpage.cpp
    Sources/Global/GlobalFunc.cpp
    Sources/Global/GlobalSettings.cpp
    Sources/SystemKit/AsulApplication.cpp

    Sources/COM_HomeWidget/T_Home.cpp
    Sources/COM_SplashWindow/T_SplashScreen.cpp
    Sources/COM_SettingWidget/T_SettingScreen.cpp
    Sources/COM_AboutWidget/T_About.cpp
    Sources/COM_IconWidget/T_Icon.cpp
    Sources/COM_IconWidget/T_IconDelegate.cpp
    Sources/COM_IconWidget/T_IconModel.cpp
    Sources/COM_ExampleWidget.template/T_Example.cpp

    Sources/CTL_AsulComboBox/AsulComboBox.cpp

    Sources/MainEntry/main.cpp
    Sources/MainEntry/asul.cpp
    
    Sources/SystemKit/BaseInclude.h
    Sources/SystemKit/BaseScrollpage.h
    Sources/Global/GlobalFunc.h
    Sources/Global/GlobalSettings.h
    Sources/SystemKit/AsulApplication.h
    Sources/COM_HomeWidget/T_Home.h
    Sources/COM_SplashWindow/T_SplashScreen.h
    Sources/COM_SettingWidget/T_SettingScreen.h
    Sources/COM_AboutWidget/T_About.h
    Sources/COM_IconWidget/T_Icon.h
    Sources/COM_IconWidget/T_IconDelegate.h
    Sources/COM_IconWidget/T_IconModel.h
    Sources/COM_ExampleWidget.template/T_Example.h

    Sources/CTL_AsulComboBox/AsulComboBox.h
    
    Sources/MainEntry/Def.h
    Sources/MainEntry/asul.h
    Sources/MainEntry/singleton.h
    Sources/MainEntry/stdafx.h

    3rd/VDX/VDX.cpp
    3rd/VDX/VDX.h
    3rd/NEEPU-SDK/NEEPULoginManager.cpp
    3rd/NEEPU-SDK/NEEPULoginManager.h
)

# 资源文件
set(RESOURCES
    AsulKitSrc.qrc
)
set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/AsulKit_zh_CN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/AsulKit_en_US.ts
)

# 添加资源文件
qt_add_resources(RESOURCE_FILES ${RESOURCES})
qt6_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

# qt6_add_translations(${PROJECT_NAME} ${TS_FILES})
# 可执行文件
add_executable(${PROJECT_NAME} WIN32
    ${SRCS}
    ${RESOURCE_FILES}
    ${TS_FILES}
    ${QM_FILES}


)

# 设置Windows资源文件
if(WIN32)
    set(RC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/IDC/res.rc)
    set_source_files_properties(${RC_FILE} PROPERTIES LANGUAGE RC)
    target_sources(${PROJECT_NAME} PRIVATE ${RC_FILE})
endif()

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ElaWidgetTools/install/include
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QT_LIBRARIES}
)

# Windows特定设置
if(WIN32)
    # 链接ElaWidgetTools库
    if(MINGW)
        target_link_libraries(${PROJECT_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/ElaWidgetTools/install/lib/libElaWidgetTools.a
        )
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/ElaWidgetTools/install/lib/ElaWidgetTools.lib
        )
    endif()

    # 部署DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/ElaWidgetTools/install/bin/ElaWidgetTools.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying ElaWidgetTools.dll to binary directory"
    )
endif()

# 安装设置
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
install(FILES ${QM_FILES} 
    DESTINATION translations
)

# 复制3rd文件夹
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd/
    DESTINATION 3rd
    PATTERN "*"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# 复制3rd文件夹到构建目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/3rd"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/3rd"
    COMMENT "Copying 3rd directory to binary directory"
)
