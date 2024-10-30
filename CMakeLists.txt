cmake_minimum_required(VERSION 3.22)
project(hazelbot VERSION 2.1.1 DESCRIPTION "A discord bot made for use in the Hazelnut Studio Discord server")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(${PROJECT_NAME}
	src/StringUtils.cpp
	src/ConfigParser.cpp
	src/BotResponses.cpp

	src/TimezoneOffsetFix.cpp
	src/Logger.cpp
	src/EmojiHandler.cpp
	
	src/Modules/ChatInteractions.cpp
	src/Modules/Counting/CountingState.cpp
	src/Modules/Counting/CountingSavesystem.cpp
	src/Modules/Counting/Counting.cpp
	src/Modules/Clip.cpp
	src/Modules/Two.cpp
	
	src/RegisterCommand.cpp
	src/Commands/CStats.cpp
	src/Commands/Quote.cpp
	src/Commands/Quote_MessageInfo.cpp
	src/Commands/Wisdom.cpp

	Hazelbot.cpp
)

# Find pre-installed D++ package (using cmake/FindCPP.cmake)
find_package(DPP REQUIRED)
target_link_libraries(${PROJECT_NAME}
	${DPP_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${DPP_INCLUDE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
