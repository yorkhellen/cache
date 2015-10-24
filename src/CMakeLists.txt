
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Istdc++ -g -Wall")
INCLUDE_DIRECTORIES(/usr/local/pvfs/include)
LINK_DIRECTORIES(/usr/local/pvfs/lib)
SET(LIBRARY  libpvfs2.so libpthread.so libcrypto.so libstdc++.so)
SET(ADD pvfsapi.c cache_log.cpp cache_alg.cpp)

SET(SRC_LIST test.cpp )
ADD_EXECUTABLE(TEST ${SRC_LIST} ${ADD})
TARGET_LINK_LIBRARIES(TEST ${LIBRARY})

SET(SERVER_LIST server.cpp)
ADD_EXECUTABLE(SERVER ${SERVER_LIST} ${ADD})
TARGET_LINK_LIBRARIES(SERVER ${LIBRARY})


