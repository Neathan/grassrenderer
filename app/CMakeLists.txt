add_executable(app "src/main.cpp" "src/app.cpp" "src/app.h" "src/shader.h" "src/shader.cpp" "src/model.h" "src/model.cpp" "src/camera.h" "src/camera.cpp" "src/generator.h" "src/generator.cpp" "src/image.h" "src/image.cpp" "src/grass.h" "src/grass.cpp" "src/world.h" "src/world.cpp" "src/frustum.h"   "src/profile.h")

set_property(TARGET app PROPERTY CXX_STANDARD 17)

target_link_libraries(app PUBLIC glad)
target_link_libraries(app PUBLIC glfw)
target_link_libraries(app PUBLIC glm)
target_link_libraries(app PUBLIC spdlog)
target_link_libraries(app PUBLIC imgui)
target_link_libraries(app PUBLIC FastNoise2)
target_link_libraries(app PUBLIC TracyClient)

target_include_directories(app PUBLIC "src")

target_compile_definitions(app PUBLIC GLFW_INCLUDE_NONE TRACY_ENABLE)


#==============================================================================
# COMPILE SHADERS
#

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/assets/shaders)
set(SHADER_BINARY_DIR $<TARGET_FILE_DIR:app>/assets/shaders)

file(GLOB SHADER_FILES
	${SHADER_SOURCE_DIR}/*.vert
	${SHADER_SOURCE_DIR}/*.frag
	${SHADER_SOURCE_DIR}/*.comp)

add_custom_target(shaders)
add_custom_target(shader_directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_BINARY_DIR}")

foreach(FILE ${SHADER_FILES})
    get_filename_component(FILENAME ${FILE} NAME)
    add_custom_command(
        TARGET shaders
        COMMAND "${CMAKE_SOURCE_DIR}/bin/glslc.exe" "--target-env=opengl" ${FILE} -o "${SHADER_BINARY_DIR}/${FILENAME}.spv"
        DEPENDS ${FILE}
        COMMENT "Compiling shader: ${FILENAME}"
    )
endforeach(FILE)


add_dependencies(shaders shader_directory)
add_dependencies(app shaders)

#==============================================================================
# COPY ASSETS
#

set(TEXTURE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/assets/textures)
set(TEXTURE_OUTPUT_DIR $<TARGET_FILE_DIR:app>/assets/textures)

add_custom_target(copy_textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_SOURCE_DIR} ${TEXTURE_OUTPUT_DIR}
)
add_dependencies(app copy_textures)
