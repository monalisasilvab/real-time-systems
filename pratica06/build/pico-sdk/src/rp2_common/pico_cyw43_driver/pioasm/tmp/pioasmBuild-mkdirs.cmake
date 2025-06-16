# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/freertos-pico/pico-sdk/tools/pioasm"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pioasm"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pioasm-install"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/monalisa/Downloads/Smestre7/sistemas-tempo-real/pratica06/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
