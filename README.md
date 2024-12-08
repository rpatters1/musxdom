# musx object model

Header-only object model for the EnigmaXml format in Finale musx files.

### Setup Instructions

Clone the git repository and add the `./src` folder to your include paths.

Include the top header in your source file.

```cpp
#include "musx/musx.h"
```

You will also need an xml parser. This repository provides implementations for [tinyxml](https://github.com/leethomason/tinyxml2) and [rapidxml](https://rapidxml.sourceforge.net/). If you wish to use one of these, define one and/or the other in your project's CMakeLists.txt file:

```cmake
target_compile_definitions(your-project PRIVATE MUSX_USE_TINYXML2) # for tinyxml2
target_compile_definitions(your-project PRIVATE MUSX_USE_RAPIDXML) # for rapidxml
```

You then must add the dependency for the one you choose to your CMakeLists.txt.

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
auto doc = DocumentFactory::create<tinyxml2::Document>(enigmaXmlBuffer); // to use tinyxml2;
auto doc = DocumentFactory::create<rapidxml::Document>(enigmaXmlBuffer); // to use rapidxml;
auto doc = DocumentFactory::create<MyCustomXmlDocument>(enigmaXmlBuffer); // to use a different xml parser
```

### To Run the Tests

You need `cmake` to build and run the tests. To configure the build directory:

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
