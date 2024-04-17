if(NOT EXISTS "/home/arne/interval/240408_TD3/D/build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/home/arne/interval/240408_TD3/D/build/install_manifest.txt\"")
endif()

file(READ "/home/arne/interval/240408_TD3/D/build/install_manifest.txt" files)
string(REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if(EXISTS "$ENV{DESTDIR}${file}")
    exec_program(
      "/usr/bin/cmake" ARGS "-E remove -f \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    if("${rm_retval}" STREQUAL 0)
    else()
      message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\": ${rm_out}")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()
