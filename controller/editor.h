#pragma once

#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "vstgui/lib/ccolor.h"

namespace CarouselReverb {

using namespace Steinberg;
using namespace VSTGUI;

/**
 * Custom editor for Carousel Reverb
 * Retro-futuristic "ancient artifact" design with 90s/2000s aesthetic
 */
class CarouselEditor : public VST3Editor {
public:
    CarouselEditor(EditController* controller, const char* templateName, 
                   const char* xmlFile);
    ~CarouselEditor() override;

protected:
    void attach(IController* controller) override;
    void draw(CDrawContext* pContext) override;
    void onIdle() override;

private:
    // Custom color palette - retro glow aesthetic
    static const CColor COLOR_BG_DARK;        // Main background
    static const CColor COLOR_METAL;          // Brushed metal
    static const CColor COLOR_GLOW_CYAN;      // Retro cyan glow
    static const CColor COLOR_GLOW_AMBER;     // Retro amber glow
    static const CColor COLOR_GLOW_GREEN;     // Terminal green
    static const CColor COLOR_TEXT;           // UI text
    static const CColor COLOR_GRID;           // Grid lines

    uint32_t mAnimationCounter = 0;
    
    void drawBackgroundArtwork(CDrawContext* pContext);
    void drawRetroGrid(CDrawContext* pContext);
    void drawGlowEffect(CDrawContext* pContext, const CRect& rect, 
                       const CColor& glowColor, float intensity);
};

} // namespace CarouselReverb
