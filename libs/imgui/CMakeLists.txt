cmake_minimum_required(VERSION 3.10)
project(imgui)

add_library(imgui
    "imgui/imgui.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
    "imgui/misc/cpp/imgui_stdlib.cpp"
)

target_compile_definitions(imgui PUBLIC GLFW_INCLUDE_NONE)

target_include_directories(imgui PUBLIC imgui/)
target_link_libraries(imgui PUBLIC glfw)
