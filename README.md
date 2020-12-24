# Overview

Desktop (for now) app to access content (data / commands / etc) in a efficient way using a tagging system.
There are 2 main components on the system, `contents` and `tags`. The `content` is referenced by one or multiple `tags`. Those `contents` can be different things like simple text, commands, notes, etc. And each `content` is defined by a type which indicates what action to take when "using" / "processing" the content.
Some examples are: 
- copy to clipbard (normal text)
- execute a command line
- ...

Here a ugly screenshot of how it looks like (right now, hopefully changes soon):
![image](https://user-images.githubusercontent.com/1697015/96559065-da446880-12bc-11eb-98fe-00e7ff55c212.png)

The main objective of this tool is to access the content extremely fast (typing as few characters as possible).

### Little bit of history

A little bit of history how this idea appear, feel free to skip this section :).

I had the idea of this tool super long time ago when in multiple occasions someone send me a link or a number or a piece of information that I knew it will be useful, but not in that right moment. So after a couple of days or weeks, when needing that information, the only thing I would remember is "I know someone told me about this" (yes, I have super bad memory). 
So I started to think, how or what I'm doing when trying to remember this things, the questions I'm doing to myself (who told me this? when? where? what?). It happens that the information I remember was basically very few keywords, sometimes the person who send me that information, the topic of what it was about, sometimes the place where I receive that information, the date, or context information when I received that information.
This is why I think `tags` are useful since are keywords, just few characters that your mind will map to much wider and detailed information (what a geek). Everyone has its own keywords/tags, even for the same thing.
There are many apps that let you tag notes, images, files etc, but they are not focused on a more general purpose / daily activities. The focus of this app is to be able to access information that you will generally do (hourly, daily, weekly, monthly) in just seconds (less than 5 :)). I know there are many many use cases for this, so I will just drop a few ones I'm personally using frequently:
- Copy (and paste) personal information into mails / messages / etc, like bank account, phone number, links, referal links, etc.
- Copy (and paste) parts of c++ code that I frequently use.
- Store some temporal scripts / command lines I would be using frequently and would dissapear from the history of the console.
- Store information a friend send me over slack / whatsapp and would be either impossible to find it after a while or struggling to remember dates / keywords (I usually use my friend's name as tag :)).
- Open browsers with some tabs already, like "work" will open all the browsers (gmail, jira, slack, github, etc) tabs at once.
- Use it for storing notes or part of documentations (or reference to it) with keywords I will remember (like: `work` `docker` `test` `script`)


## Usage 

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
