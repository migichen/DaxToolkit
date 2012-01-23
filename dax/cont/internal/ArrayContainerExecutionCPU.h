/*=========================================================================

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __dax_cont_internal_ArrayContainerExecutionCPU_h
#define __dax_cont_internal_ArrayContainerExecutionCPU_h

#include <dax/Types.h>

#include <dax/internal/DataArray.h>

#include <dax/cont/internal/IteratorContainer.h>

#include <vector>

#include <assert.h>

namespace dax {
namespace cont {
namespace internal {

/// Manages an execution environment array. In the case of this class, the
/// execution uses the same memory heap as the control.
///
template<typename T>
class ArrayContainerExecutionCPU
{
public:
  typedef T ValueType;

  /// On inital creation, no memory is allocated on the device.
  ///
  ArrayContainerExecutionCPU() { }

  /// Allocates an array on the device large enough to hold the given number of
  /// entries.
  ///
  void Allocate(dax::Id numEntries) { this->DeviceArray.resize(numEntries); }

  /// Copies the data pointed to by the passed in \c iterators (assumed to be
  /// in the control environment), into the array in the execution environment
  /// managed by this class.
  ///
  template<class IteratorType>
  void CopyFromControlToExecution(
      const dax::cont::internal::IteratorContainer<IteratorType> &);

  /// Copies the data from the array in the execution environment managed by
  /// this class into the memory passed in the \c iterators (assumed to be in
  /// the control environment).
  ///
  template<class IteratorType>
  void CopyFromExecutionToControl(
      const dax::cont::internal::IteratorContainer<IteratorType> &);

  /// Frees any resources (i.e. memory) on the device.
  ///
  void ReleaseResources() { this->Allocate(0); }

  /// Gets the vector iterators for the contained array. May be too low level
  /// to expose to everyone
  ///
  typename ::std::vector<ValueType>::iterator begin()
  {
    return this->DeviceArray.begin();
  }
  typename ::std::vector<ValueType>::const_iterator begin() const
  {
    return this->DeviceArray.begin();
  }
  typename ::std::vector<ValueType>::iterator end()
  {
    return this->DeviceArray.end();
  }
  typename ::std::vector<ValueType>::const_iterator end() const
  {
    return this->DeviceArray.end();
  }

  /// Gets a DataArray that is valid in the execution environment.
  dax::internal::DataArray<ValueType> GetExecutionArray();

  /// Gets the thrust devce_vector for the contained array. May be too low level
  /// to expose to everyone
  ///
  const ::std::vector<ValueType>& GetDeviceArray() const
    {
    return this->DeviceArray;
    }
  ::std::vector<ValueType>& GetDeviceArray()
    {
    return this->DeviceArray;
    }

private:
  ArrayContainerExecutionCPU(const ArrayContainerExecutionCPU &); // Not implemented
  void operator=(const ArrayContainerExecutionCPU &);        // Not implemented

  std::vector<ValueType> DeviceArray;
};

//-----------------------------------------------------------------------------
template<class T>
template<class IteratorType>
inline void ArrayContainerExecutionCPU<T>::CopyFromControlToExecution(
    const dax::cont::internal::IteratorContainer<IteratorType> &iterators)
{
  assert(iterators.IsValid());
  assert(iterators.GetNumberOfEntries()
         == static_cast<dax::Id>(this->DeviceArray.size()));
  std::copy(iterators.GetBeginIterator(),
            iterators.GetEndIterator(),
            this->DeviceArray.begin());
}

//-----------------------------------------------------------------------------
template<class T>
template<class IteratorType>
inline void ArrayContainerExecutionCPU<T>::CopyFromExecutionToControl(
    const dax::cont::internal::IteratorContainer<IteratorType> &iterators)
{
  assert(iterators.IsValid());
  assert(iterators.GetNumberOfEntries()
         == static_cast<dax::Id>(this->DeviceArray.size()));
  std::copy(this->DeviceArray.begin(),
            this->DeviceArray.end(),
            iterators.GetBeginIterator());
}

//-----------------------------------------------------------------------------
template<class T>
inline dax::internal::DataArray<T>
ArrayContainerExecutionCPU<T>::GetExecutionArray()
{
  ValueType *rawPointer = &this->DeviceArray[0];
  dax::Id numEntries = this->DeviceArray.size();
  return dax::internal::DataArray<ValueType>(rawPointer, numEntries);
}

}
}
}

#endif //__dax_cont_internal_ArrayContainerExecutionCPU_h
