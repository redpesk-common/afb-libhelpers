# Usage

## Installation

The afb-helpers library is available as a package for the Redpesk supported linux
distributions.


To install the native package please refer to [this chapter](host-configuration/docs/1-Setup-your-build-host.html)
in the Redpesk documentation to install the Redpesk repository for your distribution.

Then use your package manager to install the library.

### OpenSuse

```bash
sudo zypper ref
sudo zypper install afb-libhelpers-devel
```

### Fedora

```bash
sudo dnf ref
sudo dnf install afb-libhelpers-devel
```

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install afb-libhelpers-dev
```

## Add the afb-helpers as a static library to your binding

In your `config.cmake` file, add a dependency to the controller library, i.e:

```cmake
set(PKG_REQUIRED_LIST
	json-c
	afb-binding
	afb-libhelpers --> this is the afb-helpers library dependency name.
)
```

Or you can also use the [FIND_PACKAGE](https://cmake.org/cmake/help/v3.19/command/find_package.html?highlight=find_package)
CMake command to add it if you don't use the [afb-cmake-modules](cmakeafbtemplates/dev_guide/cmake-overview.html)
