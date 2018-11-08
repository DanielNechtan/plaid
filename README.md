# plaid
Lightweight browser inspired by Mosaic

Goal: A classic, Mosaic-like browser that will build with OpenBSD base+xenocara.

Libraries:

- libtls
- libX11

Features:

- pledge(2)
- unveil(2)
- TLS

Current state:

- HTTPS (libtls) only for now
- root / path only
- Xlib window draws text from index.html
- Doesn't actually parse or render anything
- Messy

cd src && make
./plaid -u https://cryogenix.net
