/*
 * Copyright(c) 2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef COMPUTE_SAMPLES_VME_INTRA_HPP
#define COMPUTE_SAMPLES_VME_INTRA_HPP

#include <vector>

#include <boost/log/sources/logger.hpp>
namespace src = boost::log::sources;

#include <boost/compute/core.hpp>

#include "application/application.hpp"
#include "yuv_utils/yuv_utils.hpp"

namespace compute_samples {
class VmeIntraApplication : public Application {
public:
  void run(std::vector<std::string> &command_line,
           src::logger &logger) override;

private:
  struct Arguments {
    bool output_bmp = false;
    std::string input_yuv_path = "";
    std::string output_yuv_path = "";
    size_t qp = 0;
    size_t width = 0;
    size_t height = 0;
    size_t frames = 0;
    bool help = false;
  };

  void
  run_vme_intra(const VmeIntraApplication::Arguments &args,
                compute::context &context, compute::command_queue &queue,
                compute::kernel &ds_kernel, compute::kernel &hme_n_kernel,
                compute::kernel &intra_kernel, Capture &capture,
                PlanarImage &planar_image, compute::image2d &src_image,
                compute::image2d &ref_image, compute::image2d &src_2x_image,
                compute::image2d &ref_2x_image, compute::image2d &src_4x_image,
                compute::image2d &ref_4x_image, compute::image2d &src_8x_image,
                compute::image2d &ref_8x_image, size_t frame_idx,
                src::logger &logger) const;
  void write_results_to_file(const cl_ulong *intra_modes,
                             const cl_uchar *intra_shapes,
                             const cl_ushort *intra_residuals,
                             const cl_ushort *inter_best_residuals,
                             uint32_t width, uint32_t height,
                             uint32_t frame_idx) const;
  Arguments parse_command_line(const std::vector<std::string> &command_line);
};
} // namespace compute_samples

#endif
