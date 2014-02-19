include_directories(. ${ok_rw_proj_SOURCE_DIR}/ok ${ok_rw_proj_SOURCE_DIR}/mcc)
link_directories(${ok_rw_proj_SOURCE_DIR}/mcc)

set(SRC ok_rw.c
		../ok/okFrontPanelDLL.cpp ../ok/okFrontPanelDLL.h
		)

add_executable (ok_rw ${SRC})

set_source_files_properties( ../ok/okFrontPanelDLL.cpp PROPERTIES LANGUAGE "C" )
if (UNIX)
	set_source_files_properties(../ok/okFrontPanelDLL.cpp PROPERTIES
		COMPILE_FLAGS "-x c")
endif (UNIX)

if (UNIX)
	target_link_libraries (ok_rw dl)
endif (UNIX)

install (TARGETS ok_rw RUNTIME DESTINATION ${BINDEST} LIBRARY DESTINATION ${LIBDEST})
