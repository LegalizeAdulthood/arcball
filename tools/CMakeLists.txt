find_package(OpenGL REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

add_executable("arcball-viewer" main.cpp)
target_link_libraries("arcball-viewer" PUBLIC wx::wxgl wx::core wx::base arcball OpenGL::GL)
set_target_properties("arcball-viewer" PROPERTIES WIN32_EXECUTABLE $<PLATFORM_ID:Windows>)
target_folder("arcball-viewer" "Tools")
