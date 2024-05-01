find_package(glm CONFIG REQUIRED)

add_library(arcball
    include/arcball/arcball.h
    arcball.cpp
)
target_include_directories(arcball PUBLIC include)
target_link_libraries(arcball PUBLIC glm::glm-header-only)
target_folder(arcball "Libraries")
