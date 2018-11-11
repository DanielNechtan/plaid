# plaid
Lightweight browser inspired by Mosaic

Goal: A classic, Mosaic-like browser that will build with OpenBSD base+xenocara.

Libraries:

- [libtls](https://www.libressl.org/)
- [libX11](https://github.com/openbsd/xenocara/tree/master/lib/libX11)
- [libtidy](https://github.com/htacg/tidy-html5) (MIT-like license)

Features:

- pledge(2)
- unveil(2)
- TLS v1.2

#### Current state

- Xlib window draws HTML after it has been parsed by libtidy
- Doesn't actually render anything
- Messy. Terrible memory management. If unveil(2) didn't exist it would 
probably be extremely expoitable.

#### Building

Build libtidy:

        cd 3rd/tidy-html5/build/cmake/
        cmake ../.. -DCMAKE_BUILD_TYPE=Release  -DBUILD_SHARED_LIB:BOOL=OFF
        make

Build plaid:

        cd src/
        make clean
        make

Running plaid:

        plaid [-u URL]

