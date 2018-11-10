#!/bin/sh

cd tidy-html5/build/cmake
cmake ../.. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIB:BOOL=OFF

