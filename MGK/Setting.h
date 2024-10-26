#pragma once

//////////////////////////////////// Window options
// window name
constexpr const char* WINDOW_NAME = "MGK-2";

// window size
constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 800;

// fullscreen option
constexpr bool FULL_SCREEN_OPTION = false;



//////////////////////////////////// Engine options
// intro screen option
constexpr bool ENABLE_INTRO_SCREEN = true;

// frame limits option
// If this option is set to 0, the engine will run at maximum framerate.
constexpr int FRAME_LIMITS = 0;



//////////////////////////////////// Development options
// console window option
constexpr bool SHOW_CONSOLE = true;

// show boundbox option
#define SHOW_BOUND_BOX



//////////////////////////////////// Scene Options
// scene layer option
// 'END' should be typed at last
enum Layer
{ LAYER_1, LAYER_2, LAYER_3, END };

// scene container reserve option
constexpr int OBJECT_LIST_RESERVE = 500;



//////////////////////////////////// Sound Options
// Max sound channel option
constexpr int MAX_CHANNEL_SIZE = 32;

// FFT Size option
constexpr int FFT_SIZE = 1024;



//////////////////////////////////// File security Options
// Application version
constexpr float APPLICATION_VERSION = 1.0;

// Data file security option
// When this option is activated, the data file is saved as an encrypted file.
// When this option is disabled, the data file is saved as an xml file.
constexpr bool USE_FILE_SECURITY = false;

// AES, IV for FileUtil
// Recommend that you make this part private or 
// modify it with the default password before distributing
constexpr unsigned char AES_KEY[16]
{ 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };

constexpr unsigned char IV_KEY[16]
{ 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36 };