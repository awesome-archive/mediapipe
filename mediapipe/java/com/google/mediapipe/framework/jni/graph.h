// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_H_
#define JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_H_

#include <jni.h>

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "mediapipe/framework/calculator_framework.h"
#ifndef MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gl_calculator_helper.h"
#endif  // !defined(MEDIAPIPE_DISABLE_GPU)
#include "absl/synchronization/mutex.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

namespace mediapipe {
namespace android {

namespace internal {
class CallbackHandler;
class PacketWithContext;
}  // namespace internal

// Graph is used to keep mediapipe related native objects into one place,
// so that we can clean up or query later.
class Graph {
 public:
  // The Packet java class name.
  static constexpr char const* kJavaPacketClassName =
      "com/google/mediapipe/framework/Packet";

  Graph();
  Graph(const Graph&) = delete;
  Graph& operator=(const Graph&) = delete;
  ~Graph();

  // Adds a callback for a given stream name.
  ::mediapipe::Status AddCallbackHandler(std::string output_stream_name,
                                         jobject java_callback);

  // Adds a packet with header callback for a given stream name.
  ::mediapipe::Status AddCallbackWithHeaderHandler(
      std::string output_stream_name, jobject java_callback);

  // Loads a binary graph from a file.
  ::mediapipe::Status LoadBinaryGraph(std::string path_to_graph);
  // Loads a binary graph from a buffer.
  ::mediapipe::Status LoadBinaryGraph(const char* data, int size);
  // Gets the calculator graph config.
  const CalculatorGraphConfig& GetCalculatorGraphConfig();

  // Runs the graph until it closes.
  // Mainly is used for writing tests.
  ::mediapipe::Status RunGraphUntilClose(JNIEnv* env);

  // The following 4 functions are used to run the graph in
  // step by step mode, the usual call sequence is like this:
  //   StartRunningGraph
  //   Loop:
  //     AddPacketToInputStream
  //   CloseInputStream
  //   WaitUtilDone
  // TODO: We need to have a synchronized wait for each step, i.e.,
  // wait until nothing is running and nothing can be scheduled.
  //
  // Starts running the graph.
  ::mediapipe::Status StartRunningGraph(JNIEnv* env);
  // Closes one input stream.
  ::mediapipe::Status CloseInputStream(std::string stream_name);
  // Closes all the graph input streams.
  ::mediapipe::Status CloseAllInputStreams();
  // Closes all the graph packet sources.
  ::mediapipe::Status CloseAllPacketSources();
  // Waits util graph is done.
  ::mediapipe::Status WaitUntilDone(JNIEnv* env);
  // Waits util graph is idle.
  ::mediapipe::Status WaitUntilIdle(JNIEnv* env);
  // Adds a packet to an input stream.
  ::mediapipe::Status AddPacketToInputStream(const std::string& stream_name,
                                             const Packet& packet);
  // Moves a packet into an input stream.
  ::mediapipe::Status AddPacketToInputStream(const std::string& stream_name,
                                             Packet&& packet);
  // Takes the MediaPipe Packet referenced by the handle, sets its timestamp,
  // and then tries to move the Packet into the given input stream.
  ::mediapipe::Status SetTimestampAndMovePacketToInputStream(
      const std::string& stream_name, int64_t packet_handle, int64_t timestamp);

  // Sets the mode for adding packets to a graph input stream.
  void SetGraphInputStreamAddMode(
      CalculatorGraph::GraphInputStreamAddMode mode);
  // Adds one input side packet.
  void SetInputSidePacket(const std::string& stream_name, const Packet& packet);

  // Adds one stream header.
  void SetStreamHeader(const std::string& stream_name, const Packet& packet);

  // Puts a mediapipe packet into the context for management.
  // Returns the handle to the internal PacketWithContext object.
  int64_t WrapPacketIntoContext(const Packet& packet);

  // Gets the shared mediapipe::GpuResources. Only valid once the graph is
  // running.
  mediapipe::GpuResources* GetGpuResources() const;

  // Adds a surface output for a given stream name.
  // Multiple outputs can be attached to the same stream.
  // Returns a native packet handle for the mediapipe::EglSurfaceHolder, or 0 in
  // case of failure.
  int64_t AddSurfaceOutput(const std::string& stream_name);

  // Sets a parent GL context to use for texture sharing.
  ::mediapipe::Status SetParentGlContext(int64 java_gl_context);

  // Sets the object for a service.
  template <typename T>
  void SetServiceObject(const GraphService<T>& service,
                        std::shared_ptr<T> object) {
    SetServicePacket(service,
                     MakePacket<std::shared_ptr<T>>(std::move(object)));
  }
  void SetServicePacket(const GraphServiceBase& service, Packet packet);

  // Cancels the currently running graph.
  void CancelGraph();

  // Returns false if not in the context.
  static bool RemovePacket(int64_t packet_handle);

  // Returns the mediapipe Packet that is referenced by the handle.
  static Packet GetPacketFromHandle(int64_t packet_handle);

  // Returns the Graph that is managing the packet.
  static Graph* GetContextFromHandle(int64_t packet_handle);

  // Invokes a Java packet callback.
  void CallbackToJava(JNIEnv* env, jobject java_callback_obj,
                      const Packet& packet);

  // Invokes a Java packet callback with header.
  void CallbackToJava(JNIEnv* env, jobject java_callback_obj,
                      const Packet& packet, const Packet& header_packet);

  ProfilingContext* GetProfilingContext();

 private:
  // Increase the graph's default executor's worker thread stack size to run
  // Java callbacks. Java's class loader may make deep recursive calls and
  // result in a StackOverflowError. The non-portable ThreadPool class in
  // thread/threadpool.h uses a default stack size of 64 KB, which is too
  // small for Java's class loader. See bug 72414047.
  void EnsureMinimumExecutorStackSizeForJava();
  void SetPacketJavaClass(JNIEnv* env);
  std::map<std::string, Packet> CreateCombinedSidePackets();

  CalculatorGraphConfig graph_;
  bool graph_loaded_;
  // Used by EnsureMinimumExecutorStackSizeForJava() to ensure that the
  // default executor's stack size is increased only once.
  bool executor_stack_size_increased_;
  // Holds a global reference to a Packet class, so that this can be
  // used from native attached thread. This is the suggested workaround for
  // jni findclass issue.
  jclass global_java_packet_cls_;
  // All mediapipe Packet managed/referenced by the context.
  // The map is used for the Java code to be able to look up the Packet
  // based on the handler(pointer).
  std::unordered_map<internal::PacketWithContext*,
                     std::unique_ptr<internal::PacketWithContext>>
      all_packets_;
  absl::Mutex all_packets_mutex_;
  // All callback handlers managed by the context.
  std::vector<std::unique_ptr<internal::CallbackHandler>> callback_handlers_;

  // mediapipe::GpuResources used by the graph.
  // Note: this class does not create a CalculatorGraph until StartRunningGraph
  // is called, and we may have to create the mediapipe::GpuResources before
  // that time, e.g. before a SurfaceOutput is associated with a Surface.
  std::shared_ptr<mediapipe::GpuResources> gpu_resources_;

  // Maps surface output names to the side packet used for the associated
  // surface.
  std::unordered_map<std::string, Packet> output_surface_side_packets_;

  // Side packets used for callbacks.
  std::map<std::string, Packet> side_packets_callbacks_;

  // Side packets set using SetInputSidePacket.
  std::map<std::string, Packet> side_packets_;

  // Service packets held here before the graph's creation.
  std::map<const GraphServiceBase*, Packet> service_packets_;

  // All headers that required by the graph input streams.
  // Note: header has to be set for the calculators that require it during
  // Open().
  std::map<std::string, Packet> stream_headers_;

  std::unique_ptr<CalculatorGraph> running_graph_;
  CalculatorGraph::GraphInputStreamAddMode graph_input_stream_add_mode_ =
      CalculatorGraph::GraphInputStreamAddMode::WAIT_TILL_NOT_FULL;
};

}  // namespace android
}  // namespace mediapipe

#endif  // JAVA_COM_GOOGLE_MEDIAPIPE_FRAMEWORK_JNI_GRAPH_H_
