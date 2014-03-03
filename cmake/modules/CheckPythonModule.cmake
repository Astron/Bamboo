# Filename: CheckPythonModule.cmake

function(check_python_module module)
  execute_process( 
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/cmake/modules/CheckPythonModule.py" "${module}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE CHECK_PYTHON_MODULE_RESULT)

  string(TOUPPER ${module} MODULE_NAME)
  if(CHECK_PYTHON_MODULE_RESULT EQUAL 0)
    set(PYTHON_${MODULE_NAME}_FOUND 1 PARENT_SCOPE)
  endif()
endfunction()
