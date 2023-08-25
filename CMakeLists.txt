cmake_minimum_required(VERSION 3.15)
project(counter-timer-vec CXX)

if (NOT TARGET counter-timer)
	find_package(counter-timer CONFIG REQUIRED)
endif()
if (NOT TARGET rcu_vector_bp)
	find_package(rcu-vector CONFIG REQUIRED COMPONENTS rcu_vector_bp)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

file(GLOB_RECURSE HEADERS "include/*")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_link_libraries(${PROJECT_NAME}
	INTERFACE
		counter-timer
		rcu_vector_bp
)

install(TARGETS ${PROJECT_NAME} DESTINATION "include/")
