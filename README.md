# Overview

Desktop (for now) app to access content (data / commands / etc) in a efficient way using a tagging system.
There are 2 main components on the system, `contents` and `tags`. The `content` is referenced by one or multiple `tags`. Those `contents` can be different things like simple text, commands, notes, etc. And each `content` is defined by a type which indicates what action to take when "using" / "processing" the content.
Some examples are: 
- copy to clipbard (normal text)
- execute a command line
- ...

Here a ugly screenshot of how it looks like:
![image](https://user-images.githubusercontent.com/1697015/96559065-da446880-12bc-11eb-98fe-00e7ff55c212.png)

The main objective of this tool is to access the content extremely fast (typing as few characters as possible).

## Features

- Key shortcut app display (instant shown).
- Fast access to content.
- Copy to clipboard tagged content.
- Execute tagged commands.
- Persistent storage.
- Encryption of content (AES-256), to securely store private content (passwords, bank account info, etc).
- ...

# Environment configuration

For now we are working only in linux (ubuntu) but this should run in all platforms without
many changes (windows / OSX / unix).

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
```

- Compile protobuf
```bash
# ensure that $ALIAS_ROOT and all env vars exists and is set
cd %ALIAS_REPO_ROOT%/third_party/protobuf/cmake && mkdir build && cd build
cmake   -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="%ALIAS_DEP_ROOT%" -Dprotobuf_BUILD_TESTS=OFF -G "MinGW Makefiles" ..
cmake --build . --target install --config Release -- -j 8
```

- compile qxtglobalshortcut dependency

```bash
cd %ALIAS_REPO_ROOT%/third_party/qxtglobalshortcut && mkdir  build && cd build
#cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%" -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
#cmake --build . --target install --config Release -- -j 8
cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%" -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles" ..
cmake --build . --target install -- -j 8
```

- Compile special dependency crossguid
```bash
cd %ALIAS_REPO_ROOT%/third_party/crossguid && mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH="%ALIAS_DEP_ROOT%" -DBUILD_SHARED_LIBS=ON -G "MinGW Makefiles" ..
cmake --build . --target install
```

#### Project

Now you should be able to compile the project.

```bash
mkdir "%ALIAS_ROOT%/build-Debug" && cd "%ALIAS_ROOT%/build-Debug"
cmake -DCMAKE_PREFIX_PATH:PATH="C:\Qt\5.12.4\mingw73_64\lib\cmake" -G "MinGW Makefiles" "%ALIAS_REPO_ROOT%"
cmake --build . 
```



# Running

## Default config

To be able to run the project we need to create some folders before being able to execute it
```bash
mkdir -p ~/alias/storage
cp $ALIAS_REPO_ROOT/resources/config/init.json ~/alias/
```

Note that you must update the `init.json` file paths to point to your local home, also, change
the `storageType` to `PERSISTENT` if you want to keep the changes on the hard drive.

to run it
```bash
cd $ALIAS_ROOT/build-Debug/src/qt_client
./qt_client
```


## Config file

When running we will be using a config file, by default will be searching in `~/alias/init.json`.
You can specify this by the first argument when running `./qt_client <path_to_config_file>`.

```js
{
  "storage": {
    // the storageType can either be MEMORY or PERSISTENT (to store files). If PERSISTENT
    // then the folder should be specified in full to where we want to store the data
    // AND SHOULD EXISTS
    "storageType": "MEMORY",
    "folder": "/home/agustin/alias/storage/"
  },
  "keyBindings": {
    // key convination we want to use for opening the main screen
    "showMainScreen": "Alt+Shift+Return"
  }
}

```


# Encryption

The encryption module is based on the https://github.com/kokke/tiny-AES-C.


# QtCreator configuration

## Running tests as target

- Add a new target on the project called test as custom executable, and set the following values:
- - command: ctest 
- - arguments: -V
- - working directory: %{buildDir}

select and run the target you should see all the test running
