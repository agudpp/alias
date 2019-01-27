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

* Add a new target on the project called test as custom executable, and set the following values:
** command: ctest 
** arguments: -V
** working directory: %{buildDir}

select and run the target you should see all the test running