# TODO: Split build rules into several individual files to make them
# more manageable.

""".bzl file for mediapipe open source build configs."""

load("@protobuf_archive//:protobuf.bzl", "cc_proto_library")
load("@protobuf_archive//:protobuf.bzl", "py_proto_library")

def mediapipe_py_proto_library(
        name,
        srcs,
        visibility,
        py_proto_deps = [],
        proto_deps = None,
        api_version = None):
    """Generate py_proto_library for mediapipe open source version.

    Args:
      name: the name of the py_proto_library.
      srcs: the .proto files of the py_proto_library for Bazel use.
      visibility: visibility of this target.
      py_proto_deps: a list of dependency labels for Bazel use; must be py_proto_library.
    """
    _ignore = [api_version, proto_deps]
    py_proto_library(
        name = name,
        srcs = srcs,
        visibility = visibility,
        default_runtime = "@protobuf_archive//:protobuf_python",
        protoc = "@protobuf_archive//:protoc",
        deps = py_proto_deps + ["@protobuf_archive//:protobuf_python"],
    )

def mediapipe_cc_proto_library(name, srcs, visibility, deps = [], cc_deps = [], testonly = 0):
    """Generate cc_proto_library for mediapipe open source version.

      Args:
        name: the name of the cc_proto_library.
        srcs: the .proto files of the cc_proto_library for Bazel use.
        visibility: visibility of this target.
        deps: a list of dependency labels for Bazel use; must be cc_proto_library.
        testonly: test only proto or not.
    """
    _ignore = [deps]
    cc_proto_library(
        name = name,
        srcs = srcs,
        visibility = visibility,
        deps = cc_deps,
        testonly = testonly,
        cc_libs = ["@protobuf_archive//:protobuf"],
        protoc = "@protobuf_archive//:protoc",
        default_runtime = "@protobuf_archive//:protobuf",
    )
