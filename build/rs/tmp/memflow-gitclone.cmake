
if(NOT "/home/rick/Desktop/Linux Overlay/build/rs/src/memflow-stamp/memflow-gitinfo.txt" IS_NEWER_THAN "/home/rick/Desktop/Linux Overlay/build/rs/src/memflow-stamp/memflow-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/rick/Desktop/Linux Overlay/build/rs/src/memflow-stamp/memflow-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/rick/Desktop/Linux Overlay/build/rs/src/memflow"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/rick/Desktop/Linux Overlay/build/rs/src/memflow'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --depth 1 --no-single-branch --progress --config "advice.detachedHead=false" "https://github.com/memflow/memflow" "memflow"
    WORKING_DIRECTORY "/home/rick/Desktop/Linux Overlay/build/rs/src"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/memflow/memflow'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 0.1.5 --
  WORKING_DIRECTORY "/home/rick/Desktop/Linux Overlay/build/rs/src/memflow"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '0.1.5'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/rick/Desktop/Linux Overlay/build/rs/src/memflow"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/rick/Desktop/Linux Overlay/build/rs/src/memflow'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/rick/Desktop/Linux Overlay/build/rs/src/memflow-stamp/memflow-gitinfo.txt"
    "/home/rick/Desktop/Linux Overlay/build/rs/src/memflow-stamp/memflow-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/rick/Desktop/Linux Overlay/build/rs/src/memflow-stamp/memflow-gitclone-lastrun.txt'")
endif()

