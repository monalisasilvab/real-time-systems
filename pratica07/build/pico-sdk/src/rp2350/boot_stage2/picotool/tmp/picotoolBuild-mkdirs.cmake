# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/_deps/picotool-src"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/_deps/picotool-build"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/_deps"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/pico-sdk/src/rp2350/boot_stage2/picotool/tmp"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/pico-sdk/src/rp2350/boot_stage2/picotool/src/picotoolBuild-stamp"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/pico-sdk/src/rp2350/boot_stage2/picotool/src"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/pico-sdk/src/rp2350/boot_stage2/picotool/src/picotoolBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/pico-sdk/src/rp2350/boot_stage2/picotool/src/picotoolBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/repositorio/pratica07/build/pico-sdk/src/rp2350/boot_stage2/picotool/src/picotoolBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
