# musx object model

Document object model for the EnigmaXml format in Finale musx files. It is compatible with the C++17 standard.

**This project is not affiliated with or endorsed by Finale or its parent company.**

- It is an independent open-source header library designed to help users access and convert their own data in the absence of Finale, which has been discontinued.
- It does not contain any Finale source code.
- It has been separately developed by analyzing the contents of EnigmaXml files and other publically available resources, such as the [PDK Framework](https://pdk.finalelua.com/) for Finale and Jari Williamsson’s [original site](https://www.finaletips.nu/frameworkref/index.html).
- Nothing in this repository circumvents digital copy protection on the Finale application.

### Setup Instructions

Include the top header in your source file.

```cpp
#include "musx/musx.h"
```

Add the libary to your project with `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
  musx
  GIT_REPOSITORY https://github.com/rpatters1/musx-object-model
  GIT_TAG main # Replace with the desired commit hash, tag, or branch
)
FetchContent_MakeAvailable(musx)

# Also add somewhere:
target_link_libraries(project PRIVATE musx) # replace "your_project" with your actual project name
```

You will also need an xml parser. This repository provides implementations for [tinyxml](https://github.com/leethomason/tinyxml2) and [rapidxml](https://rapidxml.sourceforge.net/). If you wish to use one of these, define one and/or the other in your project's CMakeLists.txt file:

```cmake
set(MUSX_USE_TINYXML2 ON CACHE BOOL "Disable tinyxml2 parsing classes" FORCE)
set(MUSX_USE_RAPIDXML ON CACHE BOOL "Enable rapidxml parsing classes" FORCE)
```

You then must add the dependency for the chosen parser(s) to your CMakeLists.txt.

For tinyxml2:

```cmake
# Add tinyxml2
FetchContent_Declare(
    tinyxml2
    URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(tinyxml2)
```

For rapidxml, you will need to download the original version from [SourceForge](https://rapidxml.sourceforge.net/). There are more recent forks of rapidxml available, but they may not work with C++17 or the implementation here. Download the zip file and unzip it to a location like `.third_party/rapidxml/`. Then you can add it to your CMakeLists.txt file with:

```cmake
# Add the include path for rapidxml
target_include_directories(your-project PRIVATE "${CMAKE_SOURCE_DIR}/third_party/rapidxml")
```

You can include them both if you wish to benchmark one against the other. If you prefer a different xml parser, you can use it by defining an implementation of `musx::xml:: IXmlAttribute`, `musx::xml::IXmlElement`, and `musx::xml::IXmlDocument` for your parser.

To create a document with a particular parser, use its `IXmlDocument` class as the template to create the document.

Examples:

```cpp
using namespace musx::factory;
using namespace musx::xml;
auto docTiny = DocumentFactory::create<tinyxml2::Document>(enigmaXmlBuffer); // to use tinyxml2;
auto docRapid = DocumentFactory::create<rapidxml::Document>(enigmaXmlBuffer); // to use rapidxml;
auto docOther = DocumentFactory::create<OtherXmlDocument>(enigmaXmlBuffer); // to use a different xml parser
```

### To Run the Tests

You need `cmake` to build and run the tests. From the directory containing this repository, configure the build directory:

```bash
cmake -S . build
```

To build the tests:

```bash
cmake --build build
```

To run the tests:

```bash
ctest --test-dir build
```

### To Build the Documentation

You need [Doxygen](https://doxygen.nl/index.html) to create the documentation. You can download and run the installer from the Doxygen site. Package managers can also install it. Once you have Doxygen you can build the documentation as follows.

```bash
cd docs
doxygen Doxyfile
```

You will then find an html website in `./docs/generated/html`. Use a web browser to view the site from your hard drive. Opening any of the `.html` pages gives you access to the entire site.
