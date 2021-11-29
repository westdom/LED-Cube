# Visual Studio Code set up:

## Extensions

Extensions can do many different things. In this case, the `Arduino` extension gives support for connecting to the Arduino board & the `C/C++` extensions gives support for their respective languages.

### Set up:

1. Navigate to the `Extensions` tab on the left hand side
2. Search and install the `Arduino`, `C/C++` & `C/C++ Extension Pack` extensions
3. Once Arduino is installed, press `F1`, and type `Arduino: Board Config`
4. Configure your board

## Libraries

Libraries are imported packages of code that allows us to use predefined functions, classes etc. In this case we need to add the Arduino library so arduino specific functions are recognised by the IDE.

### Set up:

1. In the newly created `.vscode` folder, create a file named `c_cpp_properties.json`
2. Copy in the following text (make sure to replace `{Path to Arduino}` with the proper path, for me it was `C:/Program Files (x86)/Arduino`):
 ```{
    "configurations": [
        {
            "name": "Win32",
            "compilerPath": "{Path to Arduino}/hardware/tools/avr/bin/avr-gcc.exe",
            "compilerArgs": [],
            "intelliSenseMode": "clang-x64",
            "includePath": [
                "${workspaceFolder}/**",
                "{Path to Arduino}/hardware/arduino/avr/**"
            ],
            "forcedInclude": [],
            "cStandard": "c11",
            "cppStandard": "c++17",
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "F_CPU=16000000L",
                "ARDUINO=10805",
                "ARDUINO_AVR_UNO",
                "ARDUINO_ARCH_AVR",
                "USBCON"
            ]
        }
    ],
    "version": 4
}