# Usage

## Installation

You can run the same command on a target runing a redpesk OS or in the [SDK container]({% chapter_link sdk-container-doc.overview %}) (development mode).

```bash
dnf install afb-libhelpers-devel
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
