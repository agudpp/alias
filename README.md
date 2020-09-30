# Overview

Simple application for information retrieval and tools execution for efficiency on daily usage


# Dependencies and compilation


Get all the dependencies and required libs:

```bash
sudo apt-get update &&\
sudo apt-get install cmake g++ build-essential qtbase5-dev qtbase5-private-dev uuid-dev
```

Install the qxtglobalshortcut dependency (https://github.com/hluk/qxtglobalshortcut).

```bash
mkdir tl_deps
cd tl_deps
git clone  https://github.com/hluk/qxtglobalshortcut
cd qxtglobalshortcut
make -j 4
sudo make install
sudo ldconfig
```

Get this repo and compile it

```bash
git clone https://github.com/agudpp/alias.git
cd alias
mkdir build
cd build
cmake ..
make -j 4
```

To execute you will need to add a config file like shown below and pass it as first argument to alias binary


# TODO: create user_env_vars.sh

# Configure environment
You can create a shell file to have the environment variables like `user_env_vars.sh` that
will point to your local folders:

```bash
#!/bin/bash

# will exit bash if a command doesnt succeed
set -e

export USER_ALIAS_REPO_ROOT=/home/agustin/dev/alias/alias
export USER_ALIAS_DEP_ROOT=/home/agustin/dev/alias/dependencies

```

so you can do something like
```bash
source /home/agustin/dev/alias/alias/scripts/user_env_vars.sh
```

- execute `setup.sh`

Compile protobuf
```bash
# ensure that $ALIAS_DEP_ROOT and all env vars exists and is set
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

Compile clip
```bash
# ensure that $ALIAS_DEP_ROOT and all env vars exists and is set
cd $ALIAS_REPO_ROOT/third_party/clip &&\
    mkdir -p build &&\
    cd build &&\
    cmake   -DCLIP_EXAMPLES=OFF \
            -DCLIP_TESTS=OFF \
            -DCMAKE_CXX_FLAGS="-fPIC" \
            ..

cmake --build . -j 8 &&\
  cp libclip.a $ALIAS_DEP_ROOT/lib/ &&\
  mkdir -p $ALIAS_DEP_ROOT/include/clip &&\
  cp ../clip.h $ALIAS_DEP_ROOT/include/clip/
```


# Config file


```json
{
    "backend": {
        "db_path": "path_to_where_we_will_store_data"
    }
}
```

## backend
- db_path: folder path where we will store the tags and elements (automatically generate 2 folders in there)


# QtCreator configuration

## Running tests as target

- Add a new target on the project called test as custom executable, and set the following values:
- - command: ctest 
- - arguments: -V
- - working directory: %{buildDir}

select and run the target you should see all the test running
