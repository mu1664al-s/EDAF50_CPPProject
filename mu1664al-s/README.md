# NewsServer

A simple single threaded news server that communicates with the client using a simple protocol.

## About

The server can handle multiple connections. There are two types of servers. Executable "serverinmemory" is an in memory server while "serverdisk" on the other hand uses a filesystem. The interactions with the two different types of databases are defined in the "dbinterface.h" interface. The message handler is defined in "messagehandler.h". This class pipes trough a connection instance to receive, validate, parse and send protocol defined messages.

### UML

![UML diagram](./UMLCPP.png)

## Installation

Run the following commands from here. The executables will be generated copies to the "bin" directory. A /database directory will also be generated. It can be used as the filesystem root for the serverdisk, but any other empty directory can also be used.

```bash
make all
make install
```

## Usage

### Starting the Server

```bash
./serverinmemory "port number"
```

or

```bash
./serverdisk "filesystem root path" "port number"
```

### Starting the Client

```bash
./client localhost "port number"
```

## Project Structure

```bash
.
├── bin
├── src
│   ├── build
│   │   ├── client
│   │   │   ├── client.cc
│   │   │   └── Makefile
│   │   ├── server
│   │   │   ├── Makefile
│   │   │   ├── serverbase.h
│   │   │   ├── serverdisk.cc
│   │   │   └── serverinmemory.cc
│   │   └── Makefile
│   ├── libclientserver
│   │   ├── connection.cc
│   │   ├── connectionclosedexception.h
│   │   ├── connection.h
│   │   ├── dbdisk.cc
│   │   ├── dbdisk.h
│   │   ├── dbinmemory.cc
│   │   ├── dbinmemory.h
│   │   ├── dbinterface.h
│   │   ├── Makefile
│   │   ├── message.cc
│   │   ├── message.h
│   │   ├── messagehandler.cc
│   │   ├── messagehandler.h
│   │   ├── protocol.h
│   │   ├── server.cc
│   │   └── server.h
│   ├── test
│   │   ├── Makefile
│   │   └── illegalTest.cc
│   └── Makefile
├── Makefile
├── README.md
└── UMLCPP.png
```
