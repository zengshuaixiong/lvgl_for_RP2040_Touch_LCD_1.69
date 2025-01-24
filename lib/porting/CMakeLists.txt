# Search for all source files in the current directory
# and save the names to the DIR_LCD_SRCS variable
aux_source_directory(. DIR_DIS_SRCS)

include_directories(../Config)
include_directories(../GUI)
include_directories(../LCD)
include_directories(../../lvgl)

# Generate link library
add_library(porting ${DIR_DIS_SRCS})
target_link_libraries(porting PUBLIC Config GUI LCD lvgl)
