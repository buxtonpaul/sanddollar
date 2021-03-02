cmake_minimum_required (VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
project (Sandollar)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
FIND_PACKAGE( Boost  COMPONENTS program_options filesystem  REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


add_executable (sanddollar sanddollar.cc)
TARGET_LINK_LIBRARIES( sanddollar LINK_PUBLIC ${Boost_LIBRARIES} )

install(TARGETS sanddollar)
