# FindStrophe.cmake - Try to find the Strophe library
# Once done this will define
#
#  STROPHE_FOUND - System has Strophe
#  STROPHE_INCLUDE_DIR - The Strophe include directory
#  STROPHE_LIBRARIES - The libraries needed to use Strophe
#  STROPHE_DEFINITIONS - Compiler switches required for using Strophe

#SET(STROPHE_DEFINITIONS ${PC_STROPHE_CFLAGS_OTHER})

FIND_PATH(STROPHE_INCLUDE_DIR NAMES strophe.h
   HINTS
   /usr
   /usr/local
   /opt
   )

FIND_LIBRARY(STROPHE_LIBRARIES NAMES strophe
   HINTS
   /usr
   /usr/local
   /opt
   )


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Strophe DEFAULT_MSG STROPHE_LIBRARIES STROPHE_INCLUDE_DIR)

MARK_AS_ADVANCED(STROPHE_INCLUDE_DIR STROPHE_LIBRARIES)
