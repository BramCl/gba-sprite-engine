project(motherload)

add_executable(${PROJECT_NAME}.elf
        src/main.cpp
        src/sample_start_scene.cpp
        src/sample_start_scene.h
        src/motherload_scene.cpp
        src/motherload_scene.h src/digger.h src/backgroundblocks.h src/dirtAnimation.h src/gameOverScene.cpp src/gameOverScene.h)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
