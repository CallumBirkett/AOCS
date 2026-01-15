cmake_minimum_required(VERSION 3.20)
project(aocs LANGUAGES C)

# use a fixed C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable strict warnings globally for all targets
add_compile_options(-Wall -Wextra -Werror)

# Sanitiser config
# Build-time switch to enable runtime sanitizers
# OFF by default so normal builds remain unchanged
option(AOCS_ENABLE_SANITIZERS "Enable ASan/UBSan" OFF)

# core aocs static library
add_library(aocs STATIC
    src/aocs_status.c
    src/aocs_log_csv.c
)

# public include path so executables and tests can include <aocs_*.h>
target_include_directories(aocs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# sim executable
add_executable(aocs_sim
    app/aocs_sim.c
)

target_link_libraries(aocs_sim PRIVATE aocs)


# test executable
add_executable(aocs_tests
    tests/test_smoke.c
)

target_link_libraries(aocs_tests PRIVATE aocs)

# test integration
enable_testing()
add_test(NAME smoke COMMAND aocs_tests)

# sanitiser helper function
function(aocs_enable_sanitizers target) # target becomes aocs lib, aocs sim and aocs tests
    if (AOCS_ENABLE_SANITIZERS) # option to toggle
        target_compile_options(${target} PRIVATE # adds flags when compiling .c to .o (PRIVATE only this target)
            -fsanitize=address,undefined # instrument machine code to add runtime checks
            -fno-omit-frame-pointer  # frame pointer to help walk back throught the call stack
        )
        # linker must be told to link sanitiser runtime libraries so that 
        # inserted calls (at compile) have real definitions. 
        target_link_options(${target} PRIVATE 
            -fsanitize=address,undefined    
        )
    endif()
endfunction()


# Apply to the static library so its object files are instrumented
aocs_enable_sanitizers(aocs)

# Apply to executables so sanitizer runtimes are linked
aocs_enable_sanitizers(aocs_sim)
aocs_enable_sanitizers(aocs_tests)
