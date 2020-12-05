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

For more information about how to use the app check [this link](https://github.com/agudpp/alias/wiki/Usage)

## Features

- Key shortcut app display (instant shown).
- Fast access to content.
- Copy to clipboard tagged content.
- Execute tagged commands.
- Persistent storage.
- Encryption of content (AES-256), to securely store private content (passwords, bank account info, etc).
- ...


# Building

For configuring the development environment and building take a look at [here](docs/BUILD.md)


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
