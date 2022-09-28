cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 14)

add_executable(client  csapp.h csapp.c client.c)
target_link_libraries(client pthread)

add_executable(server csapp.h csapp.c server.c)
target_link_libraries(server pthread)

add_executable(test csapp.h csapp.c test.c)
target_link_libraries(test pthread)

add_executable(producer-consumer csapp.h csapp.c producer-consumer.c)
target_link_libraries(producer-consumer pthread)

add_executable(read-write csapp.h csapp.c read-write.c)
target_link_libraries(read-write pthread)

add_executable(mult-server csapp.h csapp.c mult-server.c)
target_link_libraries(mult-server pthread)

add_executable(mult-thread-server csapp.h csapp.c sbuf.h sbuf.c mult-thread-server.c)
target_link_libraries(mult-thread-server pthread)
