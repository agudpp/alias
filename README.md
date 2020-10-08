# Overview

Desktop (for now) app to access content (data / commands / etc) in a efficient way using
a tagging system.


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

- Compile clip
```bash
# ensure that $ALIAS_DEP_ROOT and all env vars exists and is set
cd $ALIAS_REPO_ROOT/third_party/clip &&\
    mkdir -p build &&\
    cd build &&\
    cmake   -DCLIP_EXAMPLES=OFF \
            -DCLIP_TESTS=OFF \
            -DCMAKE_CXX_FLAGS="-fPIC" \
            ..

cmake --build . -- -j 8 &&\
  cp libclip.a $ALIAS_DEP_ROOT/lib/ &&\
  mkdir -p $ALIAS_DEP_ROOT/include/clip &&\
  cp ../clip.h $ALIAS_DEP_ROOT/include/clip/
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
