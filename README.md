# AFB Helpers library

You should find useful utilities to integrate in your bindings development.

This library is the successor of the old git [afb-helpers-submodule](https://gerrit.automotivelinux.org/gerrit/gitweb?p=apps%2Fapp-afb-helpers-submodule.git;a=summary)
now available as a separated library.

Please refer to the documentation in the `docs` folder for further informations.

## Installation

The afb-helpers library is integrated by default in the AGL SDK since the Guppy
version (>=7) and is also available as a package for the AGL supported linux
distributions.

You could find the SDK build from Yocto which embed the afb-helpers library
here:

* For the [releases](https://download.automotivelinux.org/AGL/release/) >= Guppy
 in the latest machine's deploy directory. (e.g for Guppy in
 `latest/<yourmachine>/deploy/sdk` directory)
* For the [master](https://download.automotivelinux.org/AGL/snapshots/master/)
 development branch, in the latest machine's deploy directory. (e.g in
 `latest/<yourmachine>/deploy/sdk` directory)

To install the native package please refer to [this chapter](../host-configuration/docs/1_Prerequisites.md)
in the AGL documentation to install the AGL repository for your distribution.

Then use your package manager to install the library.

### OpenSuse

```bash
sudo zypper ref
sudo zypper install agl-libafb-helpers-devel
```

### Fedora

```bash
sudo dnf ref
sudo dnf install agl-libafb-helpers-devel
```

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install agl-libafb-helpers-dev
```

## (Optionnal) Remove the git submodule version

If you already use the afb-helpers component but using the submodule version
then you have to get rid of it to be sure to link and use the library version.
To do so, you have to do the following:

* Deinitialize the submodule using `git`

```bash
# This example assumes that the git submodule is named app-afb-helpers-submodule
# and is located at your root project repository.
git submodule deinit app-afb-helpers-submodule
```

* Remove the submodule relatives lines from the `.gitmodules` file

```bash
vim .gitmodules
```

* Remove the `afb-helpers` target link from any CMake target you specified.
 Those lines look like:

```bash
TARGET_LINK_LIBRARIES(${TARGET_NAME}
    afb-helpers # REMOVE THIS LINE
    ${link_libraries}
    )
```

## Add the libafb-helpers as a static library to your binding

In your `config.cmake` file, add a dependency to the afb-helpers library, i.e:

```cmake
set(PKG_REQUIRED_LIST
	json-c
	afb-daemon
	afb-helpers --> this is the afb-helpers library dependency name.
)
```

Or you can also use the [FIND_PACKAGE](https://cmake.org/cmake/help/v3.6/command/find_package.html?highlight=find_package)
CMake command to add it if you don't use the [cmake-apps-module](../cmakeafbtemplates/0_Abstract.html)
