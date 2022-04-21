# CPP Project, Communication Classes

- The clientserver directory contains the communication classes Connection and Server,
  and the auxiliary classes ConnectionClosedException and Protocol. These
  classes are defined in the project description, sections 5.2 and 6.
- `make` makes the library libclientserver.a and recursively builds the
  example programs in `example`.
- The subdirectory `example` contains the example programs `myclient` and `myserver`,
  as defined in the project description, section 5.3.

## building with make

`make` in the clientserver directory builds the library (in
lib/libclientserver.a) and the example programs
myclient and myserver (in the example directory).

For cleaning, use `make clean` and `make distclean`.

## running the examples

To run the examples, open two terminal windows.

In the first one, start the server with `myserver <port>`, e.g.,

```
example/myserver 7777
```

In the other one, start the client with `myclient <server> <port>`, e.g.,

```
example/myclient localhost 7777
```

## building with cmake
There is also a CMakeLists.txt, which builds the library and the
example client and server.

Typical usage:

```
mkdir build
cd build
cmake ..
make
```

The CMakeLists.txt also includes support for installing the
example binaries. Example usage installing under /tmp:

```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/tmp ..
make
make install
```
which will install the two example programs in /tmp/bin.
(if you don't set `CMAKE_INSTALL_PREFIX`, make install will use the
default install prefix, `/usr/local`)

