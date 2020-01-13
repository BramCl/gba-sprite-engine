project(motherload)



add_executable(${PROJECT_NAME}.elf
        src/main.cpp
        src/start_screen_scene.cpp
        src/start_screen_scene.h
        src/motherload_scene.cpp
        src/motherload_scene.h
        src/digger.h
        src/backgroundblocks.h
        src/gameover_scene.cpp
        src/gameover_scene.h
        src/instruction_scene.cpp
        src/instruction_scene.h)


target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
