#pragma once

#include "pluginterfaces/base/ftypes.h"

namespace CarouselReverb {

using namespace Steinberg;

// Plugin Info
constexpr const char* PLUGIN_NAME = "Carousel Reverb";
constexpr const char* PLUGIN_VENDOR = "Looner";
constexpr const char* PLUGIN_VERSION = "1.0.0";
constexpr const char* PLUGIN_URL = "https://looner.dev";
constexpr const char* PLUGIN_EMAIL = "info@looner.dev";
constexpr const char* PLUGIN_SUBCATEGORY = "Fx|Reverb";

// Plugin IDs - Each must be globally unique
// Format: 4 bytes: each byte represents one hex digit
// These are FUID (128-bit) but we'll use simplified version
// In real code, use INLINE_UID_FROM_FUID for actual UIDs
static const FUID ProcessorUID(0x12345678, 0x9ABCDEF0, 0x11223344, 0x55667788);
static const FUID ControllerUID(0x87654321, 0xF0EDCBA9, 0x44332211, 0x88776655);

// Audio Parameters
constexpr float DEFAULT_DECAY_TIME = 2.0f;  // seconds
constexpr float DEFAULT_WET_DRY = 0.5f;     // 0.0 = dry, 1.0 = wet
constexpr float DEFAULT_TEMPO_SYNC = 1.0f; // 1 bar per layer
constexpr float DEFAULT_CROSSFADE = 0.5f;  // how much layers blend
constexpr float MAX_LAYER_COUNT = 8.0f;    // maximum carousel layers
constexpr float DEFAULT_ROTARY_SPEED = 0.5f;  // Wankel rotor speed (0.5-3.0 Hz ~ 3K-18K RPM simulation)
constexpr float DEFAULT_ROTARY_DEPTH = 0.3f;  // Wankel engine effect intensity (0-1)

// Audio processing
constexpr int32_t SAMPLES_PER_LAYER = 44100; // 1 second at 44.1kHz
constexpr int32_t MAX_LAYER_SAMPLES = 88200; // 2 seconds max

// Parameter IDs
constexpr int32_t PARAM_DECAY_TIME = 0;
constexpr int32_t PARAM_WET_DRY = 1;
constexpr int32_t PARAM_TEMPO_SYNC = 2;
constexpr int32_t PARAM_CROSSFADE = 3;
constexpr int32_t PARAM_LAYER_COUNT = 4;
constexpr int32_t PARAM_FEEDBACK = 5;
constexpr int32_t PARAM_ROTARY_SPEED = 6;
constexpr int32_t PARAM_ROTARY_DEPTH = 7;

// LFO Parameters
constexpr int32_t PARAM_LFO_RATE = 8;
constexpr int32_t PARAM_LFO_DEPTH = 9;
constexpr int32_t PARAM_LFO_WAVEFORM = 10;
constexpr int32_t PARAM_LFO_TEMPO_SYNC = 11;
constexpr int32_t PARAM_LFO_TARGET = 12;  // Which parameter to modulate (0-7)
constexpr int32_t PARAM_LFO_ENABLED = 13;

// UI sizing
constexpr int32_t UI_WIDTH = 800;
constexpr int32_t UI_HEIGHT = 600;

} // namespace CarouselReverb
