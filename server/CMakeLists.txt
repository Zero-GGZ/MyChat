cmake_minimum_required(VERSION 3.0.0)
project(mychat VERSION 0.1.0)

include(CTest)
enable_testing()
include_directories(/usr/include/mysql++/ /usr/include/mysql/ /usr/include/jsoncpp)		#添加头文件搜索路径
add_compile_options(-std=c++17)		#设置编译选项

add_executable(mychat_server server.cpp api_db.cpp api_json.cpp)
target_link_libraries(mychat_server -lmysqlpp -ljsoncpp -lpthread) #添加libmxnet.so
# g++ -o server server.cpp api_db.cpp api_json.cpp -lmysqlpp -ljsoncpp -lpthread -I/usr/include/mysql++/ -I/usr/include/mysql/ -I/usr/include/jsoncpp -std=c++11


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
