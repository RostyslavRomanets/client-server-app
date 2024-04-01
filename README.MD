# Simple client server application

This is simple app, that can transfer files from client to server.

## Requirments
- [Boost](https://www.boost.org/users/history/version_1_84_0.html)

## Getting started
First, you need to clone this repository: `git clone https://github.com/RostyslavRomanets/client-server-app`.

### Building for Windows
- Install boost library.
- When configuring Cmake, add `BOOST_ROOT` variable.
- Also, if you want to turn off warnings about version, specify `Boost_NO_WARN_NEW_VERSIONS=1` variable.
- Run Cmake and build project.

## How to use
- Start the server to accept client.
- Next, start the client: `client <filepath>`
- Now the file is transferred to server.
