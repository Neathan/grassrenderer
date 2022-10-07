add_executable(app
	"src/main.cpp"
)

target_link_libraries(app PUBLIC glad)
target_link_libraries(app PUBLIC glfw)
target_link_libraries(app PUBLIC glm)
target_link_libraries(app PUBLIC spdlog)
