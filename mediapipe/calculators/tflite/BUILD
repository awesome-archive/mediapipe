# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

licenses(["notice"])  # Apache 2.0

package(default_visibility = ["//visibility:private"])

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_cc_proto_library")

proto_library(
    name = "ssd_anchors_calculator_proto",
    srcs = ["ssd_anchors_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_custom_op_resolver_calculator_proto",
    srcs = ["tflite_custom_op_resolver_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_inference_calculator_proto",
    srcs = ["tflite_inference_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_converter_calculator_proto",
    srcs = ["tflite_converter_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_tensors_to_segmentation_calculator_proto",
    srcs = ["tflite_tensors_to_segmentation_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_tensors_to_detections_calculator_proto",
    srcs = ["tflite_tensors_to_detections_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "ssd_anchors_calculator_cc_proto",
    srcs = ["ssd_anchors_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//mediapipe:__subpackages__"],
    deps = [":ssd_anchors_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_custom_op_resolver_calculator_cc_proto",
    srcs = ["tflite_custom_op_resolver_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//mediapipe:__subpackages__"],
    deps = [":tflite_custom_op_resolver_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_converter_calculator_cc_proto",
    srcs = ["tflite_converter_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//mediapipe:__subpackages__"],
    deps = [":tflite_converter_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_tensors_to_segmentation_calculator_cc_proto",
    srcs = ["tflite_tensors_to_segmentation_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//mediapipe:__subpackages__"],
    deps = [":tflite_tensors_to_segmentation_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_inference_calculator_cc_proto",
    srcs = ["tflite_inference_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//mediapipe:__subpackages__"],
    deps = [":tflite_inference_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_tensors_to_detections_calculator_cc_proto",
    srcs = ["tflite_tensors_to_detections_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//mediapipe:__subpackages__"],
    deps = [":tflite_tensors_to_detections_calculator_proto"],
)

cc_library(
    name = "ssd_anchors_calculator",
    srcs = ["ssd_anchors_calculator.cc"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        ":ssd_anchors_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats/object_detection:anchor_cc_proto",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/port:status",
    ],
    alwayslink = 1,
)

cc_library(
    name = "tflite_custom_op_resolver_calculator",
    srcs = ["tflite_custom_op_resolver_calculator.cc"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        ":tflite_custom_op_resolver_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/port:status",
        "//mediapipe/util/tflite:cpu_op_resolver",
        "//mediapipe/util/tflite:op_resolver",
    ],
    alwayslink = 1,
)

filegroup(
    name = "anchor_golden_files",
    srcs = [
        "testdata/anchor_golden_file_0.txt",
        "testdata/anchor_golden_file_1.txt",
    ],
)

cc_test(
    name = "ssd_anchors_calculator_test",
    srcs = ["ssd_anchors_calculator_test.cc"],
    data = [":anchor_golden_files"],
    deps = [
        ":ssd_anchors_calculator",
        ":ssd_anchors_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_runner",
        "//mediapipe/framework/deps:file_path",
        "//mediapipe/framework/formats/object_detection:anchor_cc_proto",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:integral_types",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/tool:validate_type",
    ],
)

cc_library(
    name = "tflite_inference_calculator",
    srcs = ["tflite_inference_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":tflite_inference_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/util:resource_util",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
        "//mediapipe/framework/stream_handler:fixed_size_input_stream_handler",
        "//mediapipe/framework/port:ret_check",
    ] + select({
        "//mediapipe:android": [
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_buffer",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_program",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_shader",
            "//mediapipe/gpu:gl_calculator_helper",
            "//mediapipe/gpu:gpu_buffer",
            "@org_tensorflow//tensorflow/lite/delegates/gpu:gl_delegate",
        ],
        "//conditions:default": [],
    }),
    alwayslink = 1,
)

cc_library(
    name = "tflite_converter_calculator",
    srcs = ["tflite_converter_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":tflite_converter_calculator_cc_proto",
        "//mediapipe/util:resource_util",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/stream_handler:fixed_size_input_stream_handler",
        "//mediapipe/framework/tool:status_util",
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/port:ret_check",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
    ] + select({
        "//mediapipe:android": [
            "//mediapipe/gpu:gpu_buffer",
            "//mediapipe/gpu:gl_calculator_helper",
            "@org_tensorflow//tensorflow/lite/delegates/gpu:gl_delegate",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_buffer",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_program",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_shader",
        ],
        "//conditions:default": [],
    }),
    alwayslink = 1,
)

cc_library(
    name = "tflite_tensors_to_segmentation_calculator",
    srcs = ["tflite_tensors_to_segmentation_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":tflite_tensors_to_segmentation_calculator_cc_proto",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_absl//absl/types:span",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework:calculator_context",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/util:resource_util",
        "@org_tensorflow//tensorflow/lite:framework",
    ] + select({
        "//mediapipe:android": [
            "//mediapipe/gpu:gl_calculator_helper",
            "//mediapipe/gpu:gl_simple_shaders",
            "//mediapipe/gpu:gpu_buffer",
            "//mediapipe/gpu:shader_util",
            "@org_tensorflow//tensorflow/lite/delegates/gpu:gl_delegate",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_buffer",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_program",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_shader",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_texture",
        ],
        "//conditions:default": [],
    }),
    alwayslink = 1,
)

cc_library(
    name = "tflite_tensors_to_detections_calculator",
    srcs = ["tflite_tensors_to_detections_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":tflite_tensors_to_detections_calculator_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_absl//absl/types:span",
        "//mediapipe/framework/deps:file_path",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:location",
        "//mediapipe/framework/formats/object_detection:anchor_cc_proto",
        "//mediapipe/framework/port:ret_check",
        "@org_tensorflow//tensorflow/lite:framework",
    ] + select({
        "//mediapipe:android": [
            "//mediapipe/gpu:gl_calculator_helper",
            "@org_tensorflow//tensorflow/lite/delegates/gpu:gl_delegate",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_buffer",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_program",
            "@org_tensorflow//tensorflow/lite/delegates/gpu/gl:gl_shader",
        ],
        "//conditions:default": [],
    }),
    alwayslink = 1,
)

cc_test(
    name = "tflite_inference_calculator_test",
    srcs = ["tflite_inference_calculator_test.cc"],
    data = ["testdata/add.bin"],
    linkstatic = 1,
    deps = [
        ":tflite_inference_calculator",
        ":tflite_inference_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_runner",
        "//mediapipe/framework/deps:file_path",
        "//mediapipe/framework/port:gtest_main",
        "//mediapipe/framework/port:integral_types",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/tool:validate_type",
        "@org_tensorflow//tensorflow/lite:framework",
        "@org_tensorflow//tensorflow/lite/kernels:builtin_ops",
    ],
)
