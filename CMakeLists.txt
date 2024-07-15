cmake_minimum_required(VERSION 3.10)
project(ether_os)

# Tools
set(CC "x86_64-elf-gcc" CACHE STRING "Kernel compiler")
set(NASM "nasm" CACHE STRING "NASM")

# Common flags
set(CFLAGS "-g -O2 -pipe" CACHE STRING "User controllable C flags")
set(CPPFLAGS "" CACHE STRING "User controllable C preprocessor flags")
set(NASMFLAGS "-F dwarf -g" CACHE STRING "User controllable NASM flags")
set(LDFLAGS "" CACHE STRING "User controllable linker flags")

# Internal flags
set(CFLAGS "${CFLAGS} -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone")
set(CPPFLAGS "-I src/include ${CPPFLAGS} -MMD -MP")
set(LDFLAGS "${LDFLAGS} -nostdlib -Wl,-m,elf_x86_64 -Wl,-nostdlib -Wl,-pie -Wl,-z,text -Wl,-z,max-page-size=0x1000 -Wl,-T,linker.ld")
set(NASMFLAGS "${NASMFLAGS} -Wall -f elf64")

# Split into arguments
separate_arguments(CFLAGS UNIX_COMMAND "${CFLAGS}")
separate_arguments(CPPFLAGS UNIX_COMMAND "${CPPFLAGS}")
separate_arguments(LDFLAGS UNIX_COMMAND "${LDFLAGS}")
separate_arguments(NASMFLAGS UNIX_COMMAND "${NASMFLAGS}")

# Search for sources
file(GLOB_RECURSE C_FILES "src/*.c")
file(GLOB_RECURSE AS_FILES "src/*.S")
file(GLOB_RECURSE NASM_FILES "src/*.asm")

# Objects lists
set(OBJ "")
foreach (file ${C_FILES})
    get_filename_component(obj ${file} NAME_WE)
    list(APPEND OBJ "obj/${obj}.c.o")
endforeach ()
foreach (file ${AS_FILES})
    get_filename_component(obj ${file} NAME_WE)
    list(APPEND OBJ "obj/${obj}.S.o")
endforeach ()
foreach (file ${NASM_FILES})
    get_filename_component(obj ${file} NAME_WE)
    list(APPEND OBJ "obj/${obj}.asm.o")
endforeach ()

# Build rules
add_custom_command(OUTPUT bin/${PROJECT_NAME}
        COMMAND ${CC} ${CFLAGS} ${OBJ} ${LDFLAGS} -o bin/${PROJECT_NAME}
        COMMAND printf '\\003' | dd of=bin/${PROJECT_NAME} bs=1 count=1 seek=16 conv=notrunc 2>/dev/null
        DEPENDS ${OBJ} linker.ld
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Linking kernel executable")

add_custom_target(build_kernel ALL DEPENDS bin/${PROJECT_NAME})

# Compile rules for .c
foreach (file ${C_FILES})
    get_filename_component(obj ${file} NAME_WE)
    add_custom_command(OUTPUT obj/${obj}.c.o
            COMMAND ${CC} ${CFLAGS} ${CPPFLAGS} -c ${file} -o obj/${obj}.c.o
            DEPENDS ${file}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Compiling ${file} -> obj/${obj}.c.o"
            VERBATIM USES_TERMINAL)
    add_custom_target(c_compile_${obj} DEPENDS obj/${obj}.c.o)
    add_dependencies(build_kernel c_compile_${obj})
endforeach ()

# Compile rules for .S
foreach (file ${AS_FILES})
    get_filename_component(obj ${file} NAME_WE)
    add_custom_command(OUTPUT obj/${obj}.S.o
            COMMAND ${CC} ${CFLAGS} ${CPPFLAGS} -c ${file} -o obj/${obj}.S.o
            DEPENDS ${file}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Compiling ${file} -> obj/${obj}.S.o"
            VERBATIM USES_TERMINAL)
    add_custom_target(as_compile_${obj} DEPENDS obj/${obj}.S.o)
    add_dependencies(build_kernel as_compile_${obj})
endforeach ()

# Compile rules for .asm
foreach (file ${NASM_FILES})
    get_filename_component(obj ${file} NAME_WE)
    add_custom_command(OUTPUT obj/${obj}.asm.o
            COMMAND ${NASM} ${NASMFLAGS} ${file} -o obj/${obj}.asm.o
            DEPENDS ${file}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Assembling ${file} -> obj/${obj}.asm.o"
            VERBATIM USES_TERMINAL)
    add_custom_target(assemble_${obj} DEPENDS obj/${obj}.asm.o)
    add_dependencies(build_kernel assemble_${obj})
endforeach ()

# Clean
add_custom_target(dist_clean
        COMMAND ${CMAKE_COMMAND} -E rm -rf bin obj
        COMMENT "Cleaning up")
