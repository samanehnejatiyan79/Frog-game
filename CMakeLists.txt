cmake_minimum_required(VERSION 3.10)  # این دستور باید اول باشد
project(MyOpenGLGame)  # سپس دستور project باید بعد از cmake_minimum_required() بیاید

set(CMAKE_CXX_STANDARD 17)  # ارتقا به C++17 برای امکانات بهتر

# پیدا کردن OpenGL
find_package(OpenGL REQUIRED)

# پیدا کردن GLFW
find_package(GLFW3 REQUIRED)

# پیدا کردن FreeGLUT
find_package(GLUT REQUIRED)

# ایجاد فایل اجرایی از فایل main.cpp
add_executable(MyOpenGLGame main.cpp)

# لینک کردن OpenGL، GLFW و FreeGLUT به پروژه
target_link_libraries(MyOpenGLGame OpenGL::GL GLFW::GLFW GLUT::GLUT)
