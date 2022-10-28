set(elf_target ${CMAKE_PROJECT_NAME}.elf)
set(dll_name ${CMAKE_PROJECT_NAME}.dll)
set(rom_dll_name core-015-unk)

add_executable(${elf_target} ${SRCS})

target_include_directories(${elf_target} PRIVATE ${base_dir}/include)

set(dinofst ${base_bin_dir}/dinofst.exe)
set(dllimports ${base_build_dir}/DLLSIMPORTTAB.def)
set(inrom ${base_build_dir}/rom/dino.z64)
set(outrom ${base_build_dir}/rom/out.z64)
set(outdll ${base_build_dir}/out/${dll_name})
set(fst ${base_build_dir}/fst_work)
set(fstdll ${fst}/DLLS/${rom_dll_name}.dll)

add_custom_target(dll ALL
    COMMAND ${dinofst} -elf2dll -i "$<TARGET_FILE:${elf_target}>" -o "${fstdll}" -defs "${dllimports}"
    COMMENT "Building DLL -> ${fstdll}"
    DEPENDS ${elf_target})
	
add_custom_target(rom ALL
    COMMAND ${dinofst} -compile_rom -rom "${inrom}" -i "${fst}" -o "${outrom}"
    COMMENT "Building ROM... -> ${outrom}"
    DEPENDS dll)