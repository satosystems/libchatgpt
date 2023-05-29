# libchatgpt

This project is an unofficial ChatGPT API call library called libchatgpt.

## How to use

1. Add this repository as a submodule to your repository

    ```shell
    git submodule add https://github.com/satosystems/libchatgpt.git
    cd libchatgpt
    git submodule init
    git submodule update
    ```

2. Put the following in CMakeLists.txt

    ```cmake
    add_subdirectory(libchatgpt)
    ```

3. Build your project with cmake

    ```shell-session
    mkdir build
    cd build
    cmake ..
    make
    ```
