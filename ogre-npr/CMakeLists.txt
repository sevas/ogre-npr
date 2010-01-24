cmake_minimum_required(VERSION 2.8)

set (NAME ogre-npr)

# get OIS, Ogre, IOKit, Carbon
if(APPLE)
	#set (APP_TYPE MACOSX_BUNDLE)
	set (CMAKE_OSX_ARCHITECTURES "i386")	  #x86_64 ppc ppc64
	set (CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.4u.sdk")
	set (CMAKE_OSX_DEPLOYMENT_TARGET "10.4")
	set (CMAKE_CXX_COMPILER g++-4.0)
	set (CMAKE_C_COMPILER gcc-4.0)
	
	#Carbon & IOKit
	include_directories(${CMAKE_OSX_SYSROOT}/Developer/Headers/FlatCarbon)
	include_directories(${CMAKE_OSX_SYSROOT}/System/Library/Frameworks/IOKit.framework/Headers)
	find_library(CARBON_LIBRARY Carbon)
	find_library(IOKIT_LIBRARY IOKit)
	
	# Ogre
	set(OGRE_HOME /Library/Frameworks/OgreSDK)
	set(OGRE_INCLUDE_DIRS ${OGRE_HOME}/Dependencies/Ogre.framework/Headers)
	set(OGRE_FRAMEWORK ${OGRE_HOME}/Dependencies/Ogre.framework)
	
	#set(OGRE_HOME /Library/Frameworks/OgreSDK)
	#set(OGRE_INCLUDE_DIRS ${OGRE_HOME}/Dependencies/Ogre.framework/Headers)
	set(CG_FRAMEWORK ${OGRE_HOME}/Dependencies/Cg.framework)
	
	# OIS
	set(OIS_INCLUDE_DIRS ${OGRE_HOME}/Dependencies/include)
	set(OIS_LIBRARY ${OGRE_HOME}/Dependencies/lib/Release/libois.a)
	
	# ref include path for non apple-framework style include
	include_directories(${OGRE_INCLUDE_DIRS})
	include_directories(${OIS_INCLUDE_DIRS})
	
	# wtf
	#mark_as_advanced(${OGRE_FRAMEWORK})
	
	#boost, from macports
	include_directories(/opt/local/include)
endif()


#building
file(GLOB ${NAME}_SRCS src/*.cpp)
include_directories(src)

add_executable(${NAME} ${ogre-npr_SRCS})
if(APPLE)
target_link_libraries(${NAME} ${OGRE_FRAMEWORK} ${OIS_LIBRARY} ${CARBON_LIBRARY} ${IOKIT_LIBRARY})
endif()

# post build
if(APPLE)
	set_property(TARGET ${NAME} PROPERTY MACOSX_BUNDLE TRUE)
	set(${NAME}_CONTENTS_PATH ${EXECUTABLE_OUTPUT_PATH}/${NAME}.app/Contents)
	
	#symlink framework directories
	add_custom_command(TARGET ${NAME} POST_BUILD
		COMMAND mkdir ARGS -p ${${NAME}_CONTENTS_PATH}/Frameworks
		COMMAND ln ARGS -s -f ${OGRE_FRAMEWORK}  	${${NAME}_CONTENTS_PATH}/Frameworks/
		COMMAND ln ARGS -s -f ${CG_FRAMEWORK} 		${${NAME}_CONTENTS_PATH}/Frameworks/
		)
	
	#copy cfg files to Resources dir
	set(RELATIVE_CFG_PATH ../../../../../${CONFIG_FILES_PATH})
	add_custom_command(TARGET ${NAME} POST_BUILD
		COMMAND mkdir ARGS -p ${${NAME}_CONTENTS_PATH}/Resources	
		COMMAND ln ARGS -s -f ${RELATIVE_CFG_PATH}/plugins.cfg 		${${NAME}_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${RELATIVE_CFG_PATH}/resources.cfg 	${${NAME}_CONTENTS_PATH}/Resources/
	)
	message(STATUS ${${NAME}_CONTENTS_PATH})
	# link media path
	set(RELATIVE_MEDIA_PATH ../../../../../media)
	add_custom_command(TARGET ${NAME} POST_BUILD
		COMMAND ln ARGS -s -f ${RELATIVE_MEDIA_PATH} ${${NAME}_CONTENTS_PATH}/Resources/
	)
endif()