include_directories("./include/imgui")

set(IMGUI_SOURCES
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})