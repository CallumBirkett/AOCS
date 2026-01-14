cmake_minimum_required(VERSION 3.20)
project(aocs LANGUAGES C)

# Use C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for editor IntelliSense (VS Code, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Warnings-as-errors (good beginner safety net)
add_compile_options(-Wall -Wextra -Werror)

# ---- Library: libaocs ----
add_library(aocs STATIC
    src/aocs_status.c
    src/aocs_log_csv.c
)

target_include_directories(aocs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---- App: aocs_sim ----
add_executable(aocs_sim
    app/aocs_sim.c
)

target_link_libraries(aocs_sim PRIVATE aocs)

# ---- Tests: aocs_tests ----
add_executable(aocs_tests
    tests/test_smoke.c
)

target_link_libraries(aocs_tests PRIVATE aocs)

enable_testing()
add_test(NAME smoke COMMAND aocs_tests)

# ---- Optional: Sanitizers (DebugSan-like) ----
option(AOCS_ENABLE_SANITIZERS "Enable ASan/UBSan" OFF)
if (AOCS_ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()
