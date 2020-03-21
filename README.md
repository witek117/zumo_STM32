# STM32 ZUMO platform for students
<table>
  <tr>
    <td><img src="https://www.agh.edu.pl/fileadmin/default/templates/images/uczelnia/siw/znak/symetryczny/en/dwuwiersz/agh_nzw_s_en_2w_wbr_rgb_150ppi.jpg" height=480></td>
    <td><img src="https://i.imgur.com/IT10WP1.jpg" height=480></td>
  </tr>
</table>

# How to download and use
## Requirements in PATH
- CMake [https://cmake.org/](https://cmake.org/)
- program make [https://www.gnu.org/software/make/](https://www.gnu.org/software/make/)
- gcc, g++ [https://gcc.gnu.org/](https://gcc.gnu.org/)
- gcc-arm-none-eabi [https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
## Windows
- Install **git** or download desktop repository
- Install **CMake**
- Install **MinGW** with **gcc**, **g++** and make program [https://www.youtube.com/watch?v=sXW2VLrQ3Bs](https://www.youtube.com/watch?v=sXW2VLrQ3Bs)
- Install **gcc-arm-none-eabi** from [https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
- for programming with **J-Link**:
  - [https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
  - J-Link Software and Documentation Pack
  - Click for downloads
  - J-Link Software and Documentation pack for Windows
  - download latest version
- for programming with **ST-Link**:
  - [https://www.st.com/en/development-tools/stsw-link009.html](https://www.st.com/en/development-tools/stsw-link009.html)
- add to **PATH**:
  - CMake
  - MinGW/bin, 
  - GNU Tools Arm Embedded/X 20XX-qX-major/bin 
  - JLink
- Install **STM32CubeMX** [https://www.st.com/en/development-tools/stm32cubemx.html](https://www.st.com/en/development-tools/stm32cubemx.html)

## Linux
- Install git, CMake, gcc-arm-none-eabi, J-Link, ST_Link, STM32CubeMX

# Run
## CLion
- download and install CLion [https://www.jetbrains.com/clion/download/#section=windows](https://www.jetbrains.com/clion/download/#section=windows)
- File -> Open
- find first CMakeLists.txt file in zumo_STM32 directory
- **Open as Project**
- File -> Settings -> Build, Execution, Deployment -> CMake -> Build type
  - Release (recomended)
  - Debug

