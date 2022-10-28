set(base_dir ${CMAKE_CURRENT_LIST_DIR}/..)
set(base_bin_dir "${base_dir}/bin")
set(base_build_dir "${base_dir}/build")
set(base_compiler_dir "${base_bin_dir}/mips64-elf/bin")

list(APPEND CMAKE_PROGRAM_PATH ${base_compiler_dir})

set(CMAKE_TOOLCHAIN_FILE ${base_dir}/cmake/toolchain-mips64.cmake)

set(cargs "-std=c99 -EB -march=r4300 -mabi=32 -mfpxx -mno-odd-spreg -ffreestanding")
set(cargs "${cargs} -mfix4300 -shared -fPIC -mabicalls -O3 -g3 -nostdlib -G0 -fno-builtin-fprintf")
set(cargs "${cargs} -fno-builtin-fvsnprintf -fvisibility=hidden -ffunction-sections -fdata-sections")
set(cargs "${cargs} -Wno-endif-labels -mhard-float")

set(ldargs "-Wl,--undefined,dynamic_lookup,--warn-constructors,--section-start=.bss=0x00800000")
set(ldargs "${ldargs} -T ${base_bin_dir}/link_dll.ld")

set(CMAKE_CXX_FLAGS ${cargs})
set(CMAKE_C_FLAGS ${cargs})
set(CMAKE_EXE_LINKER_FLAGS ${ldargs})