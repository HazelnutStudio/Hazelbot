	# Minimum CMake version required, we'll just use the latest version.
	cmake_minimum_required(VERSION 3.22)
	# Project name, version and description
  project(hazelbot VERSION 2.0 DESCRIPTION "A discord bot made for use in the Hazelnut Studio discord server")
	 
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

	# Create an executable
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
    src/Hazelbot.cpp
)
	 
	# Find our pre-installed DPP package (using FindDPP.cmake).
	find_package(DPP REQUIRED)
	 
	# Link the pre-installed DPP package.
	target_link_libraries(${PROJECT_NAME} 
	    ${DPP_LIBRARIES}
	)

	# Include the DPP directories.
	target_include_directories(${PROJECT_NAME} PRIVATE
	    ${DPP_INCLUDE_DIR}
	)
	 
	# Set C++ version
	set_target_properties(${PROJECT_NAME} PROPERTIES
	    CXX_STANDARD 17
	    CXX_STANDARD_REQUIRED ON
	)
