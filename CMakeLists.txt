# CMakeList.txt : CMake project for ChessEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChessEngine")

# Add source to this project's executable.
#add_executable (${PROJECT_NAME} 
#	"debug.cpp"
#)

add_executable (${PROJECT_NAME} 
	"main.cpp"
	"src/ChessEngine.cpp" 
	"src/ChessBoard.cpp" 
	"src/ChessPiece.cpp"
	"src/visualize.cpp"
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChessEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

add_subdirectory(extern/SDL)
add_subdirectory(extern/SDL_image)
add_subdirectory(extern/Catch2)
add_subdirectory(extern/hat-trie)

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_SOURCE_DIR}/include
	PRIVATE extern/SDL/include
	PRIVATE extern/SDL_image/include
#	PRIVATE extern/Catch2/include
	PRIVATE extern/hat-trie/include
)

target_link_libraries(${PROJECT_NAME}
	SDL2main
	SDL2
	SDL2_image
#	Catch2
	tsl::hat_trie
)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (${PROJECT_NAME}_test 
	"test/ChessEngine.cpp"
	"test/MoveGeneration.cpp"

	"src/ChessEngine.cpp" 
	"src/ChessBoard.cpp" 
	"src/ChessPiece.cpp"
)

target_include_directories(${PROJECT_NAME}_test
	PUBLIC ${CMAKE_SOURCE_DIR}/include
	PUBLIC extern/Catch2/include
	PRIVATE extern/hat-trie/include
)

target_link_libraries(${PROJECT_NAME}_test
	Catch2::Catch2WithMain
	tsl::hat_trie
)

