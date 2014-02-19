project(ok_rw_proj)

cmake_minimum_required(VERSION 2.6)

if (WIN32)
	set (BINDEST .)
	set (LIBDEST .)
	set (SHAREDEST .)
 	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
else (WIN32)
	set (BINDEST bin)
	set (LIBDEST lib${LIB_SUFFIX})
	set (SHAREDEST share/ok_rw)
endif (WIN32)

add_subdirectory (src)
