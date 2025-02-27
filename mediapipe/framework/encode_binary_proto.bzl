"""A rule for encoding a text format protocol buffer into binary.

Example usage:

    proto_library(
        name = "calculator_proto",
        srcs = ["calculator.proto"],
    )

    encode_binary_proto(
        name = "foo_binary",
        deps = [":calculator_proto"],
        message_type = "mediapipe.CalculatorGraphConfig",
        input = "foo.pbtxt",
    )

Args:
  name: The name of this target.
  deps: A list of proto_library targets that define messages that may be
        used in the input file.
  input: The text format protocol buffer.
  message_type: The root message of the buffer.
  output: The desired name of the output file. Optional.
"""

PROTOC = "@protobuf_archive//:protoc"

def _canonicalize_proto_path_oss(all_protos, genfile_path):
    """For the protos from external repository, canonicalize the proto path and the file name.

    Returns:
       Proto path list and proto source file list.
    """
    proto_paths = []
    proto_file_names = []
    for s in all_protos:
        if s.path.startswith(genfile_path):
            repo_name, _, file_name = s.path[len(genfile_path + "/external/"):].partition("/")
            proto_paths.append(genfile_path + "/external/" + repo_name)
            proto_file_names.append(file_name)
        else:
            proto_file_names.append(s.path)
    return ([" --proto_path=" + path for path in proto_paths], proto_file_names)

def _encode_binary_proto_impl(ctx):
    """Implementation of the encode_binary_proto rule."""
    all_protos = depset()
    for dep in ctx.attr.deps:
        if hasattr(dep, "proto"):
            all_protos = depset([], transitive = [all_protos, dep.proto.transitive_sources])
    textpb = ctx.file.input
    binarypb = ctx.outputs.output or ctx.actions.declare_file(
        textpb.basename.rsplit(".", 1)[0] + ".binarypb",
        sibling = textpb,
    )

    path_list, file_list = _canonicalize_proto_path_oss(all_protos, ctx.genfiles_dir.path)

    # Note: the combination of absolute_paths and proto_path, as well as the exact
    # order of gendir before ., is needed for the proto compiler to resolve
    # import statements that reference proto files produced by a genrule.
    ctx.actions.run_shell(
        inputs = list(all_protos) + [textpb, ctx.executable._proto_compiler],
        outputs = [binarypb],
        command = " ".join(
            [
                ctx.executable._proto_compiler.path,
                "--encode=" + ctx.attr.message_type,
                "--proto_path=" + ctx.genfiles_dir.path,
                "--proto_path=.",
            ] + path_list + file_list +
            ["<", textpb.path, ">", binarypb.path],
        ),
        mnemonic = "EncodeProto",
    )
    return struct(files = depset([binarypb]))

encode_binary_proto = rule(
    implementation = _encode_binary_proto_impl,
    attrs = {
        "_proto_compiler": attr.label(
            executable = True,
            default = Label(PROTOC),
            cfg = "host",
        ),
        "deps": attr.label_list(
            providers = ["proto"],
        ),
        "input": attr.label(
            mandatory = True,
            allow_single_file = True,
        ),
        "message_type": attr.string(
            mandatory = True,
        ),
        "output": attr.output(),
    },
)

def _generate_proto_descriptor_set_impl(ctx):
    """Implementation of the generate_proto_descriptor_set rule."""
    all_protos = depset(transitive = [
        dep.proto.transitive_sources
        for dep in ctx.attr.deps
        if hasattr(dep, "proto")
    ])
    descriptor = ctx.outputs.output

    # Note: the combination of absolute_paths and proto_path, as well as the exact
    # order of gendir before ., is needed for the proto compiler to resolve
    # import statements that reference proto files produced by a genrule.
    ctx.actions.run(
        inputs = list(all_protos) + [ctx.executable._proto_compiler],
        outputs = [descriptor],
        executable = ctx.executable._proto_compiler,
        arguments = [
                        "--descriptor_set_out=%s" % descriptor.path,
                        "--absolute_paths",
                        "--proto_path=" + ctx.genfiles_dir.path,
                        "--proto_path=.",
                    ] +
                    [s.path for s in all_protos],
        mnemonic = "GenerateProtoDescriptor",
    )

generate_proto_descriptor_set = rule(
    implementation = _generate_proto_descriptor_set_impl,
    attrs = {
        "_proto_compiler": attr.label(
            executable = True,
            default = Label(PROTOC),
            cfg = "host",
        ),
        "deps": attr.label_list(
            providers = ["proto"],
        ),
    },
    outputs = {"output": "%{name}.proto.bin"},
)
