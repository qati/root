// @(#)root/tmva/tmva/dnn:$Id$
// Author: Simon Pfreundschuh 13/07/16

/*************************************************************************
 * Copyright (C) 2016, Simon Pfreundschuh                                *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

////////////////////////////////////////////////////////////////
// Defines the TDevice class which encapsules device specific //
// settings for the launching of threads.                     //
////////////////////////////////////////////////////////////////

#ifndef TMVA_DNN_ARCHITECTURES_CUDA_DEVICE
#define TMVA_DNN_ARCHITECTURES_CUDA_DEVICE

#include "cuda.h"
#include "vector_types.h" // definition of dim3
#include "CudaMatrix.h"

namespace TMVA
{
namespace DNN
{

/** TDevice
 *
 * The TDevice class provides static functions for the generation of CUDA
 * grids for kernel launches and is used to encapsulate the distribution
 * of threads and blocks over the data.
 *
 */
class TDevice
{
public:
   /* Number of threads per block along first dimensions. */
   static constexpr int BlockDimX = 1;
   /* Number of threads per block along second dimensions. */
   static constexpr int BlockDimY = 32;
   /* Resulting block size. */
   static constexpr int BlockSize = BlockDimX * BlockDimY;

   /* Return dim3 object representing the a BlockDimX x BlockDimY 2D
    * block */
   static dim3 BlockDims()
   {
      return dim3(BlockDimX, BlockDimY);
   }

   /* Return 2D dim3 object representing the block grid consisting of two-dimensional
    * BlockDimX x BlockDimY blocks covering the matrix A */
   template<typename AFloat>
   static dim3 GridDims(const TCudaMatrix<AFloat> &A)
   {
      int gridDimX = A.GetNcols() / TDevice::BlockDimX;
      if ((A.GetNcols() % TDevice::BlockDimX) != 0)
          gridDimX += 1;
      int gridDimY = A.GetNrows() / TDevice::BlockDimY;
      if ((A.GetNrows() % TDevice::BlockDimY) != 0)
          gridDimY += 1;
      return dim3(gridDimX, gridDimY);
   }

   /* Return the number of threads that will be launched for a given matrix \p A */
   template<typename AFloat>
   static int NThreads(const TCudaMatrix<AFloat> &A)
   {
      int gridDimX = A.GetNcols() / TDevice::BlockDimX;
      if ((A.GetNcols() % TDevice::BlockDimX) != 0) {
         gridDimX += 1;
      }
      int gridDimY = A.GetNrows() / TDevice::BlockDimY;
      if ((A.GetNrows() % TDevice::BlockDimY) != 0) {
         gridDimY += 1;
      }
      return gridDimX * gridDimY * TDevice::BlockDimX * TDevice::BlockDimY;
   }
};

} // namespace DNN
} // namespace TMVA

#endif
