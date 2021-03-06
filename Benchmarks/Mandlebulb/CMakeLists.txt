##=============================================================================
##
##  Copyright (c) Kitware, Inc.
##  All rights reserved.
##  See LICENSE.txt for details.
##
##  This software is distributed WITHOUT ANY WARRANTY; without even
##  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
##  PURPOSE.  See the above copyright notice for more information.
##
##  Copyright 2012 Sandia Corporation.
##  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
##  the U.S. Government retains certain rights in this software.
##
##=============================================================================

#-----------------------------------------------------------------------------
set(headers
  Mandlebulb.h
  )

#-----------------------------------------------------------------------------
set_source_files_properties(${headers} PROPERTIES HEADER_FILE_ONLY TRUE)

#-----------------------------------------------------------------------------
add_executable(MandlebulbSerial ${headers} main.cxx)
set_dax_device_adapter(MandlebulbSerial DAX_DEVICE_ADAPTER_SERIAL)
add_test(MandlebulbSerial ${EXECUTABLE_OUTPUT_PATH}/MandlebulbSerial)
target_link_libraries(MandlebulbSerial ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})

#-----------------------------------------------------------------------------
if (DAX_ENABLE_OPENMP)
  add_executable(MandlebulbOpenMP ${headers} main.cxx)
  set_dax_device_adapter(MandlebulbOpenMP DAX_DEVICE_ADAPTER_OPENMP)
  add_test(MandlebulbOpenMP ${EXECUTABLE_OUTPUT_PATH}/MandlebulbOpenMP)
  target_link_libraries(MandlebulbOpenMP ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
endif (DAX_ENABLE_OPENMP)

#-----------------------------------------------------------------------------
if (DAX_ENABLE_TBB)
  add_executable(MandlebulbTBB ${headers} main.cxx)
  set_dax_device_adapter(MandlebulbTBB DAX_DEVICE_ADAPTER_TBB)
  add_test(MandlebulbTBB ${EXECUTABLE_OUTPUT_PATH}/MandlebulbTBB)
  target_link_libraries(MandlebulbTBB ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
endif (DAX_ENABLE_TBB)

#-----------------------------------------------------------------------------
if (DAX_ENABLE_CUDA)
  dax_disable_troublesome_thrust_warnings()
  cuda_add_executable(MandlebulbCuda ${headers} main.cu)
  set_dax_device_adapter(MandlebulbCuda DAX_DEVICE_ADAPTER_CUDA)
  add_test(MandlebulbCuda ${EXECUTABLE_OUTPUT_PATH}/MandlebulbCuda)
  target_link_libraries(MandlebulbCuda
                        ${OPENGL_LIBRARIES}
                        ${GLUT_LIBRARIES}
                        ${GLEW_LIBRARIES})
endif (DAX_ENABLE_CUDA)
