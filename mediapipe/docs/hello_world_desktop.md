## Hello World for C++

1.  Ensure you have a working version of MediaPipe. See
    [installation instructions](./install.md).

2.  To run the [`hello world`] example:

    ```bash
    $ git clone https://github.com/google/mediapipe/mediapipe.git
    $ cd mediapipe

    # Need bazel flag 'MEDIAPIPE_DISABLE_GPU=1' as desktop GPU is not supported currently.
    $ bazel run --define 'MEDIAPIPE_DISABLE_GPU=1' \
        mediapipe/examples/desktop/hello_world:hello_world

    # It should print 10 rows of Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    # Hello World!
    ```

3.  The [`hello world`] example uses a simple MediaPipe graph in the
    `PrintHelloWorld()` function, defined in a [`CalculatorGraphConfig`] proto.

    ```C++
    ::mediapipe::Status PrintHelloWorld() {
      // Configures a simple graph, which concatenates 2 PassThroughCalculators.
      CalculatorGraphConfig config = ParseTextProtoOrDie<CalculatorGraphConfig>(R"(
        input_stream: "in"
        output_stream: "out"
        node {
          calculator: "PassThroughCalculator"
          input_stream: "in"
          output_stream: "out1"
        }
        node {
          calculator: "PassThroughCalculator"
          input_stream: "out1"
          output_stream: "out"
        }
      )");
    ```

    You can visualize this graph using
    [MediaPipe Visualizer](https://mediapipe-viz.appspot.com) by pasting the
    CalculatorGraphConfig content below into the visualizer. See
    [here](./visualizer.md) for help on the visualizer.

    ```bash
        input_stream: "in"
        output_stream: "out"
        node {
          calculator: "PassThroughCalculator"
          input_stream: "in"
          output_stream: "out1"
        }
        node {
          calculator: "PassThroughCalculator"
          input_stream: "out1"
          output_stream: "out"
        }
    ```

    This graph consists of 1 graph input stream (`in`) and 1 graph output stream
    (`out`), and 2 [`PassThroughCalculator`]s connected serially.

    ![hello_world.cc graph](./images/hello_world_graph.png){width="200"}

4.  Before running the graph, an `OutputStreamPoller` object is connected to the
    output stream in order to later retrieve the graph output, and a graph run
    is started with [`StartRun`].

    ```c++
    CalculatorGraph graph;
    RETURN_IF_ERROR(graph.Initialize(config));
    ASSIGN_OR_RETURN(OutputStreamPoller poller,
                     graph.AddOutputStreamPoller("out"));
    RETURN_IF_ERROR(graph.StartRun({}));
    ```

5.  The example then creates 10 packets (each packet contains a string "Hello
    World!" with Timestamp values ranging from 0, 1, ... 9) using the
    [`MakePacket`] function, adds each packet into the graph through the `in`
    input stream, and finally closes the input stream to finish the graph run.

    ```c++
    for (int i = 0; i < 10; ++i) {
      RETURN_IF_ERROR(graph.AddPacketToInputStream("in", MakePacket<std::string>("Hello World!").At(Timestamp(i))));
    }
    RETURN_IF_ERROR(graph.CloseInputStream("in"));
    ```

6.  Through the `OutputStreamPoller` object the example then retrieves all 10
    packets from the output stream, gets the string content out of each packet
    and prints it to the output log.

    ```c++
    mediapipe::Packet packet;
    while (poller.Next(&packet)) {
      LOG(INFO) << packet.Get<string>();
    }
    ```

[`hello world`]: https://github.com/google/mediapipe/tree/master/mediapipe/examples/desktop/hello_world/hello_world.cc
[`CalculatorGraphConfig`]: https://github.com/google/mediapipe/tree/master/mediapipe/framework/calculator.proto
[`PassThroughCalculator`]: https://github.com/google/mediapipe/tree/master/mediapipe/calculators/core/pass_through_calculator.cc
[`MakePacket`]: https://github.com/google/mediapipe/tree/master/mediapipe/framework/packet.h
[`StartRun`]: https://github.com/google/mediapipe/tree/master/mediapipe/framework/calculator_graph.h
