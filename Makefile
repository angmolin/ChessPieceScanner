gxx = g++
gxx_opts = -O2
gxx_lib_opts = -c
gxx_libs = -ljpeg

src_dir = ./src
obj_dir = ./obj
bin_dir = ./bin

utils                        = utils
utils_obj                    = $(utils).o
utils_src                    = $(utils).cpp

module_configuration         = configuration
module_webcamV4L2            = webcamV4L2
module_serialport            = serialport
module_terminal              = terminal
module_terminal_table        = terminalTable
module_image_jpeg            = imageJPEG

module_configuration_obj     = $(module_configuration).o
module_webcamV4L2_obj        = $(module_webcamV4L2).o
module_serialport_obj        = $(module_serialport).o
module_terminal_obj          = $(module_terminal).o
module_terminal_table_obj    = $(module_terminal_table).o
module_image_jpeg_obj        = $(module_image_jpeg).o

module_configuration_src     = $(module_configuration).cpp
module_webcamV4L2_src        = $(module_webcamV4L2).cpp
module_serialport_src        = $(module_serialport).cpp
module_terminal_src          = $(module_terminal).cpp
module_terminal_table_src    = $(module_terminal_table).cpp
module_image_jpeg_src        = $(module_image_jpeg).cpp

module_configuration_src_dir = $(src_dir)/configuration
module_webcamV4L2_src_dir    = $(src_dir)/webcamV4L2
module_serialport_src_dir    = $(src_dir)/serialport
module_terminal_src_dir      = $(src_dir)/terminal

module_image_src_dir         = $(src_dir)/image
module_image_jpeg_src_dir    = $(module_image_src_dir)/jpeg

all: chesspiecescanner

chesspiecescanner: mkdir_bin module_utils module_configuration module_webcamV4L2 module_serialport module_terminal module_terminal_table module_image_jpg
	$(gxx) $(gxx_opts) $(obj_dir)/$(utils_obj) $(obj_dir)/$(module_configuration_obj) $(obj_dir)/$(module_webcamV4L2_obj) $(obj_dir)/$(module_serialport_obj) $(obj_dir)/$(module_terminal_obj) $(obj_dir)/$(module_terminal_table_obj) $(obj_dir)/$(module_image_jpeg_obj) $(gxx_libs) $(src_dir)/main.cpp -o $(bin_dir)/$@

phony: mkdir_obj mkdir_bin module_utils module_webcamV4L2 module_serialport module_terminal mkdir_obj_image module_image_jpg clean

mkdir_obj:
	-mkdir -p $(obj_dir)

mkdir_bin:
	-mkdir -p $(bin_dir)

module_utils: $(utils_obj)

$(utils_obj): mkdir_obj
	$(gxx) $(gxx_opts) $(gxx_lib_opts) $(src_dir)/$(utils_src) -o $(obj_dir)/$@

module_configuration: $(module_configuration_obj)

$(module_configuration_obj): mkdir_obj
	$(gxx) $(gxx_opts) $(gxx_lib_opts) $(module_configuration_src_dir)/$(module_configuration_src) -o $(obj_dir)/$@

module_webcamV4L2: $(module_webcamV4L2_obj)

$(module_webcamV4L2_obj): mkdir_obj
	$(gxx) $(gxx_opts) $(gxx_lib_opts) $(module_webcamV4L2_src_dir)/$(module_webcamV4L2_src) -o $(obj_dir)/$@

module_serialport: $(module_serialport_obj)

$(module_serialport_obj): mkdir_obj
	$(gxx) $(gxx_opts) $(gxx_lib_opts) $(module_serialport_src_dir)/$(module_serialport_src) -o $(obj_dir)/$@

module_terminal: $(module_terminal_obj)

$(module_terminal_obj): mkdir_obj
	$(gxx) $(gxx_opts) $(gxx_lib_opts) $(module_terminal_src_dir)/$(module_terminal_src) -o $(obj_dir)/$@

module_terminal_table: $(module_terminal_table_obj)

$(module_terminal_table_obj): mkdir_obj
	$(gxx) $(gxx_opts) $(gxx_lib_opts) $(module_terminal_src_dir)/$(module_terminal_table_src) -o $(obj_dir)/$@

module_image_jpg: $(module_image_jpeg_obj)

$(module_image_jpeg_obj): mkdir_obj
	$(gxx) $(gxx_opts) $(gxx_lib_opts) $(module_image_jpeg_src_dir)/$(module_image_jpeg_src) -o $(obj_dir)/$@

clean:
	-rm -R -f $(obj_dir)
	-rm -R -f $(bin_dir)