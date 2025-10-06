add_library("graphics-gems" INTERFACE
Ball.c
Ball.h
BallAux.c
BallAux.h
BallMath.c
BallMath.h
Body.c
Body.h
Demo.c
Makefile
README
)
set_source_files_properties(Ball.c BallAux.c BallMath.c Body.c Demo.c PROPERTIES HEADER_FILE_ONLY TRUE)
target_include_directories("graphics-gems" INTERFACE .)
set_target_properties("graphics-gems" PROPERTIES EXCLUDE_FROM_ALL EXCLUDE_FROM_DEFAULT_BUILD)
target_folder("graphics-gems" "Graphics Gems IV")
