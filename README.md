# lcdlib

## Instalation

1) add the library as a git submodule
2) add `include(lcdlib/lcdlib.cmake)` into the CMakeLists.txt file beneath `mbed_configure_app_target`
3) add `include_directories(lcdlib/include)` beneath that
4) add `${LCDLIB_SRC}` into the target\_sources

## Usage

```cpp
#include "mbed.h"
#include <lcd.hpp>

int main() {
    software_lcd lcd;
    lcd.set_flags(LCD_CURSOR | LCD_CURSOR_BLINK);
    lcd.clear();
    lcd.puts("Hello world");

    while (true) {}
}
```
