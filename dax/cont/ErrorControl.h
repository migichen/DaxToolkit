//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2012 Sandia Corporation.
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//=============================================================================
#ifndef __dax_cont_ErrorControl_h
#define __dax_cont_ErrorControl_h

#include <dax/cont/Error.h>

namespace dax {
namespace cont {

/// The superclass of all exceptions thrown from within the Dax control
/// environment.
///
class ErrorControl : public dax::cont::Error
{
protected:
  ErrorControl() { }
  ErrorControl(const std::string message) : Error(message) { }
};

}
} // namespace dax::cont

#endif //__dax_cont_ErrorControl_h
