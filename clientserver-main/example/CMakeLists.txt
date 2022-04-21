macro(add_program TESTNAME)
  # create an exectuable with project-defined flags
  add_executable(${TESTNAME} ${ARGN})

  target_compile_options(${TESTNAME}
    PRIVATE
      ${clientserver_sanitizer_options}
  )

  target_compile_definitions(${TESTNAME}
    PRIVATE
      ${clientserver_config_defines}
  )

  target_include_directories(${TESTNAME}
    PUBLIC
      ${clientserver_public_include_dirs}
    PRIVATE
      ${clientserver_private_include_dirs}
  )

  target_link_libraries(${TESTNAME}
    PUBLIC
      clientserver
    PRIVATE
      ${clientserver_sanitizer_options}
  )
endmacro()

add_program(myserver myserver.cc)
add_program(myclient myclient.cc)

install(TARGETS myserver myclient)
