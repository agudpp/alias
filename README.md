tag-linker
==========

Tool to be used to link "elements" with tags or links


Dependencies and compilation
==========

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
git clone https://github.com/agudpp/tag-linker.git
cd tag-linker
mkdir build
cd build
cmake ..
make -j 4
```

To execute you will need to add a config file like shown below and pass it as first argument to tag_be binary


Config file
=======

```json
{
    "backend": {
        "db_path": "path_to_where_we_will_store_data"
    }
}
```

TODOS
======

* (x) Complete documentation
* (x) add script for configuring and installing dependencies





