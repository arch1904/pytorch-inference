//
// Created by Aman LaChapelle on 5/26/17.
//
// pytorch_inference
// Copyright (c) 2017 Aman LaChapelle
// Full license at pytorch_inference/LICENSE.txt
//

#ifndef PYTORCH_INFERENCE_PRODUCT_IMPL_HPP
#define PYTORCH_INFERENCE_PRODUCT_IMPL_HPP

// ArrayFire
#include <arrayfire.h>

// Project
#include "../utils.hpp"

namespace pytorch::impl {

  inline af::array prodn(const std::vector<af::array> &inputs,
                        const int &dim){

    int n_tensors = inputs.size();
    af::array out = inputs[0];
#pragma omp target device(0) map(out, inputs)
    for (int i = n_tensors-1; i > 0; i--){
      out *= inputs[i];
    }

    return out;
  }

} // pytorch::impl

#endif //PYTORCH_INFERENCE_PRODUCT_IMPL_HPP
