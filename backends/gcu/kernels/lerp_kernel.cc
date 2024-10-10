// Copyright (c) 2024 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "common/gcu_op_runner.h"
#include "kernels/funcs/gcu_kernel_funcs.h"

namespace custom_kernel {

template <typename T, typename Context>
void LerpKernel(const Context& dev_ctx,
                const phi::DenseTensor& x,
                const phi::DenseTensor& y,
                const phi::DenseTensor& weight,
                phi::DenseTensor* out) {
  PADDLE_GCU_KERNEL_TRACE("lerp");
  dev_ctx.template Alloc<T>(out);
  if (LaunchAOTKernel()) {
    VLOG(0) << "LerpKernel, x:" << x;
    VLOG(0) << "LerpKernel, y:" << y;
    VLOG(0) << "LerpKernel, weight:" << weight;
    auto w =
        custom_kernel::Broadcast(dev_ctx, weight, phi::vectorize(out->dims()));
    LAUNCH_TOPSATENOP(topsatenLerp, dev_ctx, *out, x, y, w);

  } else {  // kernel impl base on JIT
    THROW_JIT_UNIMPLEMENTED();
  }
}

}  // namespace custom_kernel

PD_REGISTER_PLUGIN_KERNEL(lerp,
                          gcu,
                          ALL_LAYOUT,
                          custom_kernel::LerpKernel,
                          float,
                          phi::dtype::float16) {}