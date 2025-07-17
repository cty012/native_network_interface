cmake_minimum_required(VERSION 3.10)
project(network_interface C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define the library target without sources initially
add_library(network_interface SHARED)

# Add platform-specific source files conditionally
if(WIN32)
    # This block is used when compiling on Windows
    target_sources(network_interface PRIVATE
        src/windows/network_interface.c
        # TODO: other potential windows files
    )
    # Might need to link against the IP Helper library
    target_link_libraries(network_interface PRIVATE Iphlpapi.lib)

elseif(UNIX)
    # This block is used for Linux, macOS, iOS, and Android.
    target_sources(network_interface PRIVATE
        src/posix/network_interface.c
        src/posix/name.c
        src/posix/ip_address.c
        src/posix/mac_address.c
    )
endif()

# PUBLIC headers are in the 'include' directory.
# Clients linking to this library will get this path.
target_include_directories(network_interface
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include> # Path after installation
)

# Test
add_executable(test test/main.c)
target_link_libraries(test PRIVATE network_interface)

# --- Installation Rules ---
# This tells CMake to install the library and its public header.
include(GNUInstallDirs)
install(TARGETS network_interface
    EXPORT network_interface-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# Install only the public header file.
install(FILES include/network_interface.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
