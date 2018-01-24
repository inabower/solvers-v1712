// These are the command-line flags the program can understand.
// They define where the graph and input data is located, and what kind of
// input the model expects. If you train your own model, or use something
// other than inception_v3, then you'll need to update these.
tensorflow::string rootPath = cwd();
tensorflow::string image = rootPath + "/data/grace_hopper.jpg";
tensorflow::string graph = rootPath + "/data/inception_v3_2016_08_28_frozen.pb";
tensorflow::string labels = rootPath + "/data/imagenet_slim_labels.txt";
tensorflow::int32 input_width = 299;
tensorflow::int32 input_height = 299;
float input_mean = 0;
float input_std = 255;
tensorflow::string input_layer = "input";
tensorflow::string output_layer = "InceptionV3/Predictions/Reshape_1";
bool self_test = false;
tensorflow::string root_dir = "";
std::vector<tensorflow::Flag> flag_list = {
    tensorflow::Flag("image", &image, "image to be processed"),
    tensorflow::Flag("graph", &graph, "graph to be executed"),
    tensorflow::Flag("labels", &labels, "name of file containing labels"),
    tensorflow::Flag("input_width", &input_width, "resize image to this width in pixels"),
    tensorflow::Flag("input_height", &input_height,
         "resize image to this height in pixels"),
    tensorflow::Flag("input_mean", &input_mean, "scale pixel values to this mean"),
    tensorflow::Flag("input_std", &input_std, "scale pixel values to this std deviation"),
    tensorflow::Flag("input_layer", &input_layer, "name of input layer"),
    tensorflow::Flag("output_layer", &output_layer, "name of output layer"),
    tensorflow::Flag("self_test", &self_test, "run a self test"),
    tensorflow::Flag("root_dir", &root_dir,
         "interpret image and graph file names relative to this directory"),
};
tensorflow::string usage = tensorflow::Flags::Usage(argv[0], flag_list);
const bool parse_result = tensorflow::Flags::Parse(&argc, argv, flag_list);
if (!parse_result) {
  LOG(ERROR) << usage;
  return -1;
}

// We need to call this to set up global state for TensorFlow.
tensorflow::port::InitMain(argv[0], &argc, &argv);
if (argc > 1) {
  LOG(ERROR) << "Unknown argument " << argv[1] << "\n" << usage;
  return -1;
}

// First we load and initialize the model.
std::unique_ptr<tensorflow::Session> session;
tensorflow::string graph_path = tensorflow::io::JoinPath(root_dir, graph);
tensorflow::Status load_graph_status = LoadGraph(graph_path, &session);
if (!load_graph_status.ok()) {
  LOG(ERROR) << load_graph_status;
  return -1;
}

// Get the image from disk as a float array of numbers, resized and normalized
// to the specifications the main graph expects.
std::vector<tensorflow::Tensor> resized_tensors;
tensorflow::string image_path = tensorflow::io::JoinPath(root_dir, image);
tensorflow::Status read_tensor_status =
    ReadTensorFromImageFile(image_path, input_height, input_width, input_mean,
                            input_std, &resized_tensors);
if (!read_tensor_status.ok()) {
  LOG(ERROR) << read_tensor_status;
  return -1;
}
const tensorflow::Tensor& resized_tensor = resized_tensors[0];

// Actually run the image through the model.
std::vector<tensorflow::Tensor> outputs;
tensorflow::Status run_status = session->Run({{input_layer, resized_tensor}},
                                 {output_layer}, {}, &outputs);
if (!run_status.ok()) {
  LOG(ERROR) << "Running model failed: " << run_status;
  return -1;
}

// This is for automated testing to make sure we get the expected result with
// the default settings. We know that label 653 (military uniform) should be
// the top label for the Admiral Hopper image.
if (self_test) {
  bool expected_matches;
  tensorflow::Status check_status = CheckTopLabel(outputs, 653, &expected_matches);
  if (!check_status.ok()) {
    LOG(ERROR) << "Running check failed: " << check_status;
    return -1;
  }
  if (!expected_matches) {
    LOG(ERROR) << "Self-test failed!";
    return -1;
  }
}

// Do something interesting with the results we've generated.
tensorflow::Status print_status = PrintTopLabels(outputs, labels, image, graph);
if (!print_status.ok()) {
  LOG(ERROR) << "Running print failed: " << print_status;
  return -1;
}
