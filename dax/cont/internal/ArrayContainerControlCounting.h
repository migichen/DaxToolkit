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
#ifndef __dax_cont_internal_ArrayContainerControlCounting_h
#define __dax_cont_internal_ArrayContainerControlCounting_h

#include <dax/cont/ArrayContainerControlImplicit.h>
#include <dax/cont/ArrayPortal.h>
#include <dax/cont/internal/IteratorFromArrayPortal.h>

namespace dax {
namespace cont {
namespace internal{

/// \brief An implicit array portal that returns an counting value.
template <class CountingValueType>
class ArrayPortalCounting
{
public:
  typedef CountingValueType ValueType;

  DAX_EXEC_CONT_EXPORT
  ArrayPortalCounting() :
  StartingValue(),
  LastIndex(0)
  {  }

  DAX_EXEC_CONT_EXPORT
  ArrayPortalCounting(ValueType startingValue, dax::Id numValues) :
  StartingValue(startingValue),
  LastIndex(numValues)
  {  }

  DAX_EXEC_CONT_EXPORT
  dax::Id GetNumberOfValues() const { return this->LastIndex; }

  DAX_EXEC_CONT_EXPORT
  ValueType Get(dax::Id index) const { return StartingValue+index; }

  typedef dax::cont::internal::IteratorFromArrayPortal<
          ArrayPortalCounting < CountingValueType> > IteratorType;

  DAX_CONT_EXPORT
  IteratorType GetIteratorBegin() const
  {
    return IteratorType(*this);
  }

  DAX_CONT_EXPORT
  IteratorType GetIteratorEnd() const
  {
    return IteratorType(*this, this->LastIndex);
  }

private:
  CountingValueType StartingValue;
  dax::Id LastIndex;
};

struct ArrayContainerControlTagCounting
{
};

template< typename ConstantValueType>
class ArrayContainerControl<
    ConstantValueType,
    dax::cont::internal::ArrayContainerControlTagCounting >
{
public:
  typedef ConstantValueType ValueType;
  typedef dax::cont::internal::ArrayPortalCounting<ConstantValueType> PortalConstType;

  // This is meant to be invalid. Because ConstantValue arrays are read only, you
  // should only be able to use the const version.
  struct PortalType {
    typedef void *ValueType;
    typedef void *IteratorType;
  };

  // All these methods do nothing but raise errors.
  PortalType GetPortal() {
    throw dax::cont::ErrorControlBadValue("ConstantValue arrays are read-only.");
  }
  PortalConstType GetPortalConst() const {
    // This does not work because the ArrayHandle holds the constant
    // ArrayPortal, not the container.
    throw dax::cont::ErrorControlBadValue(
          "ConstantValue container does not store array portal.  "
          "Perhaps you did not set the ArrayPortal when "
          "constructing the ArrayHandle.");
  }
  dax::Id GetNumberOfValues() const {
    // This does not work because the ArrayHandle holds the constant
    // ArrayPortal, not the container.
    throw dax::cont::ErrorControlBadValue(
          "ConstantValue container does not store array portal.  "
          "Perhaps you did not set the ArrayPortal when "
          "constructing the ArrayHandle.");
  }
  void Allocate(dax::Id daxNotUsed(numberOfValues)) {
    throw dax::cont::ErrorControlBadValue("ConstantValue arrays are read-only.");
  }
  void Shrink(dax::Id daxNotUsed(numberOfValues)) {
    throw dax::cont::ErrorControlBadValue("ConstantValue arrays are read-only.");
  }
  void ReleaseResources() {
    throw dax::cont::ErrorControlBadValue("ConstantValue arrays are read-only.");
  }
};

template<typename T, class DeviceAdapterTag>
class ArrayTransfer<
    T, ArrayContainerControlTagCounting, DeviceAdapterTag>
{
private:
  typedef ArrayContainerControlTagCounting  ArrayContainerControlTag;
  typedef dax::cont::internal::ArrayContainerControl<T,ArrayContainerControlTag>
                                                    ContainerType;

public:
  typedef T ValueType;

  typedef typename ContainerType::PortalType PortalControl;
  typedef typename ContainerType::PortalConstType PortalConstControl;
  typedef PortalControl PortalExecution;
  typedef PortalConstControl PortalConstExecution;

  ArrayTransfer() : PortalValid(false) {  }

  DAX_CONT_EXPORT dax::Id GetNumberOfValues() const {
    DAX_ASSERT_CONT(this->PortalValid);
    return this->Portal.GetNumberOfValues();
  }

  DAX_CONT_EXPORT void LoadDataForInput(PortalConstControl portal) {
    this->Portal = portal;
    this->PortalValid = true;
  }

  DAX_CONT_EXPORT void LoadDataForInPlace(
      ContainerType &daxNotUsed(controlArray))
  {
    throw dax::cont::ErrorControlBadValue(
          "ConstantValue arrays cannot be used for output or in place.");
  }

  DAX_CONT_EXPORT void AllocateArrayForOutput(
      ContainerType &daxNotUsed(controlArray),
      dax::Id daxNotUsed(numberOfValues))
  {
    throw dax::cont::ErrorControlBadValue(
          "ConstantValue arrays cannot be used for output.");
  }
  DAX_CONT_EXPORT void RetrieveOutputData(
      ContainerType &daxNotUsed(controlArray)) const
  {
    throw dax::cont::ErrorControlBadValue(
          "ConstantValue arrays cannot be used for output.");
  }

  template <class IteratorTypeControl>
  DAX_CONT_EXPORT void CopyInto(IteratorTypeControl dest) const
  {
    DAX_ASSERT_CONT(this->PortalValid);
    std::copy(this->Portal.GetIteratorBegin(),
              this->Portal.GetIteratorEnd(),
              dest);
  }

  DAX_CONT_EXPORT void Shrink(dax::Id daxNotUsed(numberOfValues))
  {
    throw dax::cont::ErrorControlBadValue("ConstantValue arrays cannot be resized.");
  }

  DAX_CONT_EXPORT PortalExecution GetPortalExecution()
  {
    throw dax::cont::ErrorControlBadValue(
          "ConstantValue arrays are read-only.  (Get the const portal.)");
  }
  DAX_CONT_EXPORT PortalConstExecution GetPortalConstExecution() const
  {
    DAX_ASSERT_CONT(this->PortalValid);
    return this->Portal;
  }

  DAX_CONT_EXPORT void ReleaseResources() {  }

private:
  PortalConstExecution Portal;
  bool PortalValid;
};


}
}
}

#endif //__dax_cont_internal_ArrayContainerControlCounting_h
