# Building / compiling

This project requires:

- g++ compiler with c++17 standard support (mingw64-builds on win)
- cmake
- qt5


## Unix

### Repo and basics

Get all the dependencies and required libs:

```bash
sudo apt-get update &&\
sudo apt-get install cmake g++ build-essential qtbase5-dev qtbase5-private-dev
```

Set the main environment variable we will be using for the repo and more (you can set this
on the `~/.bashrc`.

```bash
# ALIAS root folder where you will create all the project related files
export ALIAS_ROOT=/home/agustin/dev/alias

# sub env vars
export ALIAS_REPO_ROOT=$ALIAS_ROOT/alias
export ALIAS_DEP_ROOT=$ALIAS_ROOT/dependencies
```

Get this repo and compile it

```bash
# ensure you have the ALIAS_ROOT env var (i.e. source ~/.bashrc)
cd $ALIAS_ROOT
git clone https://github.com/agudpp/alias.git
cd alias
git submodule update --init
```


### Compile

To compile, make sure you have downloaded the repo and configured the environment.
If you have multiple compiler you can export them before running all the following:

```bash
export CC=/usr/bin/gcc-8
export CXX=/usr/bin/g++-8
```


#### Third party

- Create the dependencies folder
```bash
mkdir -p $ALIAS_DEP_ROOT
```

- Compile protobuf
```bash
# ensure that $ALIAS_ROOT and all env vars exists and is set
cd $ALIAS_REPO_ROOT/third_party/protobuf/cmake &&\
    mkdir -p build &&\
    cd build &&\
    cmake   -Dprotobuf_BUILD_SHARED_LIBS=ON \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_INSTALL_PREFIX="$ALIAS_DEP_ROOT" \
            -Dprotobuf_BUILD_TESTS=OFF \
            ..

cmake --build . --target install --config Release -- -j 8
```


- compile qxtglobalshortcut dependency

```bash
cd $ALIAS_REPO_ROOT/third_party/qxtglobalshortcut &&\
  mkdir -p build &&\
  cd build &&\
  cmake -DCMAKE_INSTALL_PREFIX:PATH=$ALIAS_DEP_ROOT \
        -DBUILD_SHARED_LIBS=ON \
        -DCMAKE_BUILD_TYPE=Release \
        ..
cmake --build . --target install --config Release -- -j 8
```


- Compile dependency crossguid

```bash
# Release
mkdir -p $ALIAS_REPO_ROOT/third_party/crossguid/build && cd $ALIAS_REPO_ROOT/third_party/crossguid/build
cmake -DCMAKE_INSTALL_PREFIX:PATH=$ALIAS_DEP_ROOT \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=ON \
      ..
cmake --build . --target install --config Release -- -j 8

# debug
cmake -DCMAKE_INSTALL_PREFIX:PATH=$ALIAS_DEP_ROOT \
      -DCMAKE_BUILD_TYPE=Debug \
      -DBUILD_SHARED_LIBS=ON \
      ..
cmake --build . --target install --config Debug -- -j 8
```


#### Project

Now you should be able to compile the project.

```bash
mkdir -p $ALIAS_ROOT/build-Debug && cd $ALIAS_ROOT/build-Debug
cmake $ALIAS_REPO_ROOT
make -j
```

## Windows

Get all the dependencies and required libs:

- qt5
- cmake
- g++
- git
- mingw64-builds

(I use qtcreator for development.)

Set the user environment variables:

- The root environment variable: `ALIAS_ROOT` -> `C:\dev\alias` (**put your root folder here, I pull the project in C:\dev\alias\alias**)
- `ALIAS_REPO_ROOT` -> `%ALIAS_ROOT%\alias`
- `ALIAS_DEP_ROOT` -> `%ALIAS_ROOT%\dependencies`

Get this repo and compile it

```bash
# ensure you have the ALIAS_ROOT env var (note that we assume we are using the mingw64-builds console (windows one))
cd %ALIAS_ROOT%
git clone https://github.com/agudpp/alias.git
cd alias
git submodule update --init
```

### Compile

To compile, make sure you have downloaded the repo and configured the environment.

#### Third party

- Create the dependencies folder
```bash
mkdir %ALIAS_DEP_ROOT%
mkdir %ALIAS_DEP_ROOT%/Debug
mkdir %ALIAS_DEP_ROOT%/Release
```

- Compile protobuf
```bash
# ensure that $ALIAS_ROOT and all env vars exists and is set
cd %ALIAS_REPO_ROOT%/third_party/protobuf/cmake && mkdir build && cd build
#compile release
cmake   -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%ALIAS_DEP_ROOT%/Release/" -Dprotobuf_BUILD_TESTS=OFF -G "MinGW Makefiles" ..
cmake --build . --target install --config Release -- -j 8

# Debug we will use the same than release since we do not care to debug this
cmake   -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%ALIAS_DEP_ROOT%/Debug/" -Dprotobuf_BUILD_TESTS=OFF -G "MinGW Makefiles" ..
cmake --build . --target install --config Release -- -j 8


```

- compile qxtglobalshortcut dependency

```bash
cd %ALIAS_REPO_ROOT%/third_party/qxtglobalshortcut && mkdir  build && cd build
#cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%" -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
#cmake --build . --target install --config Release -- -j 8

# Compile release
cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%/Release" -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles" ..
cmake --build . --target install --config Release -- -j 8

# Compile debug
del *.* /Q
cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%/Debug" -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles" ..
cmake --build . --target install --config Debug -- -j 8

```

- Compile special dependency crossguid
```bash
# Release
cd %ALIAS_REPO_ROOT%/third_party/crossguid && mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%/Release" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles" ..
cmake --build . --target install --config Release -- -j 8

# Debug
del *.* /Q
cmake -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%/Debug" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles" ..
cmake --build . --target install --config Debug -- -j 8

```

#### Project

Now you should be able to compile the project.


Release

```bash
mkdir "%ALIAS_ROOT%/build-Release" && cd "%ALIAS_ROOT%/build-Release"
cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH="C:/alias/" -G "MinGW Makefiles" "%ALIAS_REPO_ROOT%"
cmake --build . --target install --config Release -- -j 8
```

Debug

```bash
mkdir "%ALIAS_ROOT%/build-Debug" && cd "%ALIAS_ROOT%/build-Debug"
cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH="C:/alias/" -G "MinGW Makefiles" "%ALIAS_REPO_ROOT%"
cmake --build . --target install -- -j 8
```


