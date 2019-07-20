# CLAP — Command Line Argument Parser

![GitHub](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)
![GitHub](https://img.shields.io/badge/License-MIT-brightgreen.svg)

Minimalistic C++17 command line parser.

### Features

- Only 2 files are needed:
  - [`clap.hpp`](clap.hpp)
  - [`clap.cpp`](clap.cpp)
- Just 4 functions:
  -  `parse`
  -  `has_flag`
  -  `has_data`
  -  `get`
- Supports both Windows and Linux style (with minor tweaks)

### Examples

```cpp
#include <clap.hpp>

int main(int argc, char** argv)
{
    switch(env::os)
    {
    case env::os::WINDOWS:
        clap::parse(argc, argv, clap::style::WINDOWS);
        break;
    case env::os::LINUX:
        clap::parse(argc, argv, clap::style::UNIX);
        break;
    default:
        return -1;
    }
    
    if(clap::has_flag("h") || clap::has_flag("help"))
    {
        // print help
    }
    
    if(clap::has_flag("outname") && clap::has_data("outname"))
    {
        auto const& outdata = *clap::get("outname");
        // use outdata
    }
    else
    {
        // error: 'outname' is required
        return -1
    }
    
    if(auto const* param_data = clap::get("param"); param_data)
    {
        // use param_data
    }
}
```

### Argument Style Convention

The style convention is used to:
- remove flag prefixes such as <kbd>/</kbd>, <kbd>-</kbd> and <kbd>--</kbd>,
- to understand the data delimiter, <kbd>:</kbd> for Windows and <kbd>=</kbd> for Unix

Multiple data for the same flag is divided by a <kbd>,</kbd>

#### Windows

```powershell
clap.exe /? /OutFile:C:\Users\uname\Desktop\new_file.txt /D /H /I:include\,local\private\include,some\path\include
```

By calling `clap::parse(argc, arv, clap::style::WINDOWS)` you'll populate a table like this:

| flag | data |
|:----:|:----:|
|  `?`   |      |
|  `OutFile` | `C:\Users\uname\Desktop\new_file.txt` |
|  `D`   |      |
|  `H`   |      |
|  `I`   | `include\`<br>`local\private\include`<br>`some\path\include` |

#### Unix

```bash
clap -h --help --value=42 --include-path=/usr/include,/usr/local/include -xvf --data=10,11,12,13,14,15
```

Unix style has the following tweaks:
- only flags that starts with <kbd>--</kbd> can have data
- each letter after <kbd>-</kbd> will be treated as a single flag

By calling `clap::parse(argc, arv, clap::style::UNIX)` you'll populate a table like this:

| flag | data |
|:----:|:----:|
| `h` |  |
| `help` |  |
| `value` | `42` |
| `include-path` | `/usr/include`<br>`/usr/local/include` |
| `x` |  |
| `v` |  |
| `f` |  |
| `data` | `10`<br>`11`<br>`12`<br>`13`<br>`14`<br>`15` |