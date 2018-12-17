tag-linker
==========

Tool to be used to link "elements" with tags or links


Dependencies
============

* Qt 5.X
* CMake
* C++11 +
* boost (only for boost::split (algorithm/string)) should be removed soon
* qxtglobalshortcut: https://github.com/hluk/qxtglobalshortcut (build and install)



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




Install all
==========

sudo apt-get install cmake g++ build-essential qtbase5-dev 

mkdir tl_deps
cd tl_deps
git clone  https://github.com/hluk/qxtglobalshortcut
cd qxtglobalshortcut
make -j 4
sudo make install
git clone git@github.com:agudpp/tag-linker.git
cd tag-linker
mkdir build
cd build
cmake ..
make -j 4



