// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_DNN_SRC_CUDA_GRID_STRIDE_RANGE_HPP
#define OPENCV_DNN_SRC_CUDA_GRID_STRIDE_RANGE_HPP

#include "types.hpp"

#include <cuda_runtime.h>

namespace cv { namespace dnn { namespace cuda4dnn { namespace csl { namespace device {

    namespace detail {
        using dim3_member_type = decltype(dim3::x);

        template <int>  __device__ dim3_member_type getGridDim();
        template <> inline __device__ dim3_member_type getGridDim<0>() { return gridDim.x; }
        template <> inline __device__ dim3_member_type getGridDim<1>() { return gridDim.y; }
        template <> inline __device__ dim3_member_type getGridDim<2>() { return gridDim.z; }

        template <int> __device__ dim3_member_type getBlockDim();
        template <> inline __device__ dim3_member_type getBlockDim<0>() { return blockDim.x; }
        template <> inline __device__ dim3_member_type getBlockDim<1>() { return blockDim.y; }
        template <> inline __device__ dim3_member_type getBlockDim<2>() { return blockDim.z; }

        using uint3_member_type = decltype(uint3::x);

        template <int> __device__ uint3_member_type getBlockIdx();
        template <> inline __device__ uint3_member_type getBlockIdx<0>() { return blockIdx.x; }
        template <> inline __device__ uint3_member_type getBlockIdx<1>() { return blockIdx.y; }
        template <> inline __device__ uint3_member_type getBlockIdx<2>() { return blockIdx.z; }

        template <int> __device__ uint3_member_type getThreadIdx();
        template <> inline __device__ uint3_member_type getThreadIdx<0>() { return threadIdx.x; }
        template <> inline __device__ uint3_member_type getThreadIdx<1>() { return threadIdx.y; }
        template <> inline __device__ uint3_member_type getThreadIdx<2>() { return threadIdx.z; }
    }

    template <int dim, class index_type = device::index_type, class size_type = device::size_type>
    class grid_stride_range_generic {
    public:
        __device__ grid_stride_range_generic(index_type to_) : from(0), to(to_) { }
        __device__ grid_stride_range_generic(index_type from_, index_type to_) : from(from_), to(to_) { }

        class iterator
        {
        public:
            __device__ iterator(index_type pos_) : pos(pos_) {}

            /* these iterators return the index when dereferenced; this allows us to loop
             * through the indices using a range based for loop
             */
            __device__ index_type operator*() const { return pos; }

            __device__ iterator& operator++() {
                pos += detail::getGridDim<dim>() * static_cast<index_type>(detail::getBlockDim<dim>());
                return *this;
            }

            __device__ bool operator!=(const iterator& other) const {
                /* NOTE HACK
                ** 'pos' can move in large steps (see operator++)
                ** expansion of range for loop uses != as the loop conditioion
                ** => operator!= must return false if 'pos' crosses the end
                */
                return pos < other.pos;
            }

        private:
            index_type pos;
        };

        __device__ iterator begin() const {
            using detail::getBlockDim;
            using detail::getBlockIdx;
            using detail::getThreadIdx;
            return iterator(from + getBlockDim<dim>() * getBlockIdx<dim>() + getThreadIdx<dim>());
        }

        __device__ iterator end() const {
            return iterator(to);
        }

    private:
        index_type from, to;
    };

    using grid_stride_range_x = grid_stride_range_generic<0>;
    using grid_stride_range_y = grid_stride_range_generic<1>;
    using grid_stride_range_z = grid_stride_range_generic<2>;
    using grid_stride_range = grid_stride_range_x;

}}}}} /* namespace cv::dnn::cuda4dnn::csl::device */

#endif /* OPENCV_DNN_SRC_CUDA_GRID_STRIDE_RANGE_HPP */
