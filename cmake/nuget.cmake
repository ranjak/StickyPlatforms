# Find/download nuget and get required packages.

find_program(NUGET nuget)

if (NOT NUGET)
  message(STATUS "Downloading nuget...")
  file(DOWNLOAD "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe" "${CMAKE_BINARY_DIR}\\nuget.exe")
  set(NUGET "${CMAKE_BINARY_DIR}\\nuget.exe")
endif()

configure_file("config/packages.config" "${CMAKE_BINARY_DIR}/packages.config" COPYONLY)
execute_process(COMMAND ${NUGET} restore "${CMAKE_BINARY_DIR}\\stickyplatforms.sln")