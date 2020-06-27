cmake_minimum_required(VERSION 3.5)
project(YourApp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_PREFIX_PATH "/opt/qt/5.6/gcc_64/")#указать путь к  Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)#Q_OBJECT - для использования в проекте механизма сигналов и слотов.
#set(CMAKE_AUTOUIC ON)#для поддержки форм созданных с помощью QtDesigner
find_package( Qt5Core REQUIRED )# по аналогии с *.pro QT += gui core
find_package( Qt5Widgets REQUIRED )#виджеты
find_package( Qt5Network REQUIRED )#Сеть
find_package( Qt5Sql REQUIRED )#поддержка СУБД
find_package( Qt5Xml REQUIRED )#поддержка Xml
find_package( Qt5PrintSupport REQUIRED )#
find_package( Qt5OpenGL REQUIRED)

file(GLOB SRC_FILES "inc/*.h" "src/*.cpp")

include_directories(inc)

add_executable(YourApp main.cpp ${SRC_FILES})
target_link_libraries(YourApp Qt5::Widgets GL Qt5::OpenGL Qt5::Core Qt5::Network Qt5::Sql    #указываем библиотеки
        #${Boost_LIBRARIES}
        )# с которыми будем линковать бинарник