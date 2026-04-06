# Carousel Reverb VST 3 Plugin

A spatially creative reverb effect that generates a new "layer" with each bar of music, creating a carousel of reverb tails that fade beneath new layers. Perfect for adding space and dimension to vocal and instrumental tracks.

## Features

- **Bar-Sync Carousel Reverb**: New reverb layer created each bar of music
- **Tempo-Aware**: Automatically syncs with DAW's BPM
- **Customizable Decay**: Control how long each layer fades out
- **Multi-Layer Stacking**: Up to 8 simultaneous reverb layers
- **Wet/Dry Mix**: Full control over effect blend
- **Feedback Control**: Adjust reverb tail density

## Project Structure

```
looner/
├── include/              # Header files
│   ├── processor.h       # Main audio processor
│   ├── controller.h      # Parameter controller/UI logic
│   ├── carousel.h        # Carousel reverb algorithm
│   └── constants.h       # Constants and configuration
├── src/                  # Implementation files
│   ├── processor.cpp
│   ├── controller.cpp
│   └── entry.cpp         # Plugin entry point
├── processor/            # Processor-specific code (future)
├── controller/           # Controller/UI code (future)
├── resources/            # Plugin resources (icons, presets)
├── build/                # Build output directory
└── CMakeLists.txt        # CMake build configuration
```

## Prerequisites

1. **VST 3 SDK** - Download from [Steinberg](https://github.com/steinbergmedia/vst3sdk)
2. **CMake** - Version 3.16 or higher
3. **C++ Compiler**:
   - Windows: Visual Studio 2019+ with C++ support
   - macOS: Xcode 11+
   - Linux: GCC 9+ or Clang 9+

## Setup Instructions

### 1. Get the VST 3 SDK

```bash
cd c:\Users\chaly\looner
git clone https://github.com/steinbergmedia/vst3sdk.git vst3sdk
cd vst3sdk
git submodule update --init --recursive
```

### 2. Configure CMake

```bash
cd c:\Users\chaly\looner
mkdir build
cd build
cmake .. -DSMTG_SDK_PATH="../vst3sdk"
```

### 3. Build the Plugin

**Windows (Visual Studio):**
```bash
cmake --build . --config Release
```

**macOS/Linux:**
```bash
cmake --build . --config Release -- -j$(nproc)
```

### 4. Install Plugin

**Windows:**
```
Copy: build\Release\carouselreverb.vst3
To: C:\Program Files\Common Files\VST3\
```

**macOS:**
```
Copy: build/Release/carouselreverb.vst3
To: ~/Library/Audio/Plug-Ins/VST3/
```

**Linux:**
```bash
sudo cp build/carouselreverb.vst3 ~/.vst3/
```

## Parameters

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| **Decay Time** | 0.0 - 5.0 sec | 2.0 s | How long each layer fades out |
| **Wet/Dry** | 0% - 100% | 50% | Effect/dry signal balance |
| **Tempo Sync** | 0.5 - 4.0 bars | 1.0 bar | Layer creation interval |
| **Crossfade** | 0.0 - 1.0 | 0.5 | Layer blending amount |
| **Layer Count** | 2 - 8 | 4 | Number of simultaneous layers |
| **Feedback** | 0% - 95% | 50% | Internal reverb feedback |

## How to Use

1. **Load** the plugin in your DAW as an effect on a track or bus
2. **Play** music - new reverb layers are created each bar
3. **Adjust Decay Time** to control how long reverb tails persist
4. **Increase Layer Count** for thicker, more complex reverb
5. **Adjust Feedback** for more resonant or pristine reverb character

## Development Notes

### Architecture

- **Processor** (`processor.h/cpp`): Handles all audio processing
  - Reads parameter changes from DAW
  - Accesses tempo/timing information
  - Manages the carousel reverb engine
  - Outputs processed audio

- **Controller** (`controller.h/cpp`): Manages UI and parameters
  - Defines all plugin parameters
  - Handles parameter display/formatting
  - Manages UI state

- **Carousel Engine** (`carousel.h`): Core reverb algorithm
  - `ReverbLayer`: Individual reverb buffer with fade control
  - `CarouselReverb`: Manages multiple layers, fade timing, and mixing

### Building in Debug Mode

```bash
cmake --build build --config Debug
```

### Running with Logging

Edit `constants.h` to enable debug output, then rebuild:
```cpp
#define CAROUSEL_DEBUG 1
```

## Troubleshooting

### Plugin not appearing in DAW
- Verify plugin is in correct VST3 folder
- For Windows, check `C:\Program Files\Common Files\VST3\`
- Scan/rescan plugins in DAW
- Check DAW is running 64-bit (if plugin built as 64-bit)

### Build errors
- Ensure VST 3 SDK is at `./vst3sdk`
- On Windows, use Visual Studio 2019+
- On macOS, update Xcode: `xcode-select --install`

### Audio artifacts
- Reduce Feedback parameter
- Lower Decay Time
- Reduce Layer Count
- Increase buffer size in DAW

## License

TBD - Add your license here

## Author

Looner - https://looner.dev

## References

- [VST 3 Documentation](https://steinbergmedia.github.io/vst3_doc/)
- [Steinberg VST 3 SDK](https://github.com/steinbergmedia/vst3sdk)
- Reverb algorithms inspired by Freeverb and Dattorro reverb designs
