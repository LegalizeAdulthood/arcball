add_library("graphics-gems" STATIC
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
target_include_directories("graphics-gems" PRIVATE .)
set_target_properties("graphics-gems" PROPERTIES EXCLUDE_FROM_ALL EXCLUDE_FROM_DEFAULT_BUILD)
target_folder("graphics-gems" "Graphics Gems IV")
