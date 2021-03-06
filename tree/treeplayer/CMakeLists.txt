############################################################################
# CMakeLists.txt file for building ROOT tree/treeplayer package
# @author Pere Mato, CERN
############################################################################

set(libname TreePlayer)

# TBranchProxyTemplate.h is only used by selectors, to verify that the selector
# source matches the ROOT interface. It should not end up in the dictionary nor
# in the PCH.
ROOT_GLOB_HEADERS(dictHeaders inc/*.h inc/ROOT/*.h)
list(REMOVE_ITEM dictHeaders ${CMAKE_SOURCE_DIR}/tree/treeplayer/inc/TBranchProxyTemplate.h)

ROOT_GLOB_SOURCES(sources src/*.cxx)

if(NOT imt)
  list(REMOVE_ITEM dictHeaders ${CMAKE_CURRENT_SOURCE_DIR}/inc/ROOT/TTreeProcessor.h)
  list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/TTreeProcessor.cxx)
else()
  include_directories(${TBB_INCLUDE_DIRS})
endif()

ROOT_GENERATE_DICTIONARY(G__${libname} ${dictHeaders} MODULE ${libname} LINKDEF LinkDef.h OPTIONS "-writeEmptyRootPCM")


ROOT_LINKER_LIBRARY(${libname} ${sources} G__${libname}.cxx LIBRARIES ${TBB_LIBRARIES} DEPENDENCIES Tree Graf3d Graf Hist Gpad RIO MathCore)
ROOT_INSTALL_HEADERS()


#---Extra rules-------------------------------------------------------
if(APPLE)
  if(GCC_MAJOR EQUAL 4 AND GCC_MINOR EQUAL 0 AND GCC_PATCH EQUAL 1)
    set_source_files_properties(src/TTreeFormula.cxx COMPILE_FLAGS "-O0" )
  endif()
  if(ICC_GE_9  EQUAL 10)
    set_source_files_properties(src/TTreeFormula.cxx COMPILE_FLAGS "-O0" )
  endif()
endif()

