file(GLOB_RECURSE STATEMACHINE_SOURCES "src/*.*")


add_library(statemachine "${STATEMACHINE_SOURCES}")
target_include_directories(statemachine PUBLIC include ../motordriver/include)

