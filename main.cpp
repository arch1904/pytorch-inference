#include <iostream>

#include "include/utils.hpp"
#include "include/py_object.hpp"
#include "include/ops.hpp"
#include "include/layers.hpp"
#include "include/inference_engine.hpp"

int main() {

  pycpp::python_home = "../scripts";
  pycpp::py_object test ("test");
  test("save_tensor", {pycpp::to_python(1), // out_filters
                       pycpp::to_python(3), // in_filters
                       pycpp::to_python(3),
                       pycpp::to_python(5),
                       pycpp::to_python("filts.dat")});

  test("save_tensor", {pycpp::to_python(1),
                       pycpp::to_python(1), // out_filters
                       pycpp::to_python(1),
                       pycpp::to_python(1),
                       pycpp::to_python("bias.dat")});

  test("save_tensor", {pycpp::to_python(2), // batch
                       pycpp::to_python(3), // in_filters
                       pycpp::to_python(6),
                       pycpp::to_python(6),
                       pycpp::to_python("img.dat")});

  test("save_tensor", {pycpp::to_python(1),  // no batch
                       pycpp::to_python(1),  // no filters
                       pycpp::to_python(3),  // output size
                       pycpp::to_python(8),  // input size
                       pycpp::to_python("lin_weight.dat")});

  test("save_tensor", {pycpp::to_python(1),  // no batch
                       pycpp::to_python(1),  // no filters
                       pycpp::to_python(3),  // output size
                       pycpp::to_python(1),  // vector
                       pycpp::to_python("lin_bias.dat")});

  PyObject *i = test("load_tensor", {pycpp::to_python("img.dat")});

  PyObject *pto = test("test_conv", {pycpp::to_python("filts.dat"),
                                        pycpp::to_python("bias.dat"), pycpp::to_python("img.dat"),
                                        pycpp::to_python("lin_weight.dat"), pycpp::to_python("lin_bias.dat")});

  auto image = from_numpy((PyArrayObject *)i, 4, {2, 3, 6, 6});
  auto pytorch_out = from_numpy((PyArrayObject *)pto, 4, {2, 3, 1, 1});
  pytorch_out = af::reorder(pytorch_out, 2, 1, 0, 3);

  pytorch::conv_params_t params = {3, 5, 1, 1, 0, 0};

  pytorch::Conv2d conv(params, "filts.dat", {1, 3, 3, 5}, "bias.dat", {1, 1, 1, 1});
  pytorch::Linear lin("lin_weight.dat", {1, 1, 3, 8}, "lin_bias.dat", {1, 1, 3, 1});
  pytorch::Hardtanh hardtanh(-1, 1);

  pytorch::inference_engine engine;
  engine.add_layer(conv);
  engine.add_layer(lin);
  engine.add_layer(hardtanh);
  auto output = engine.forward(image);

//  auto output = pytorch::conv2d(params, filters, bias, image);
//  output = pytorch::linear(lin_w, lin_b, output);  // not working for some reason...why not

  af_print(pytorch_out - output);

  return 0;
}