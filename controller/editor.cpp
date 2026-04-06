#include "editor.h"
#include "vstgui/lib/cbitmap.h"
#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/cstring.h"
#include <cmath>

namespace CarouselReverb {

using namespace VSTGUI;

// Retro-futuristic color palette
const CColor CarouselEditor::COLOR_BG_DARK = CColor(15, 18, 25, 255);        // Deep space black
const CColor CarouselEditor::COLOR_METAL = CColor(60, 65, 75, 255);          // Brushed metal
const CColor CarouselEditor::COLOR_GLOW_CYAN = CColor(0, 200, 255, 200);     // Neon cyan
const CColor CarouselEditor::COLOR_GLOW_AMBER = CColor(255, 150, 0, 180);    // Warm amber
const CColor CarouselEditor::COLOR_GLOW_GREEN = CColor(0, 255, 100, 180);    // Terminal green
const CColor CarouselEditor::COLOR_TEXT = CColor(200, 210, 220, 255);        // Cool gray text
const CColor CarouselEditor::COLOR_GRID = CColor(40, 60, 80, 80);            // Grid overlay

CarouselEditor::CarouselEditor(EditController* controller, const char* templateName,
                               const char* xmlFile)
    : VST3Editor(controller, templateName, xmlFile) {}

CarouselEditor::~CarouselEditor() {}

void CarouselEditor::attach(IController* controller) {
    VST3Editor::attach(controller);
}

void CarouselEditor::draw(CDrawContext* pContext) {
    // Draw custom background
    drawBackgroundArtwork(pContext);
    
    // Draw grid overlay
    drawRetroGrid(pContext);
    
    // Draw animated glow effects
    float glowIntensity = 0.3f + 0.2f * std::sin(mAnimationCounter * 0.1f);
    drawGlowEffect(pContext, getViewSize(), COLOR_GLOW_CYAN, glowIntensity);
    
    // Call parent draw for UI elements
    VST3Editor::draw(pContext);
}

void CarouselEditor::onIdle() {
    VST3Editor::onIdle();
    mAnimationCounter++;
    if (mAnimationCounter > 360)
        mAnimationCounter = 0;
    invalidRect(getViewSize());
}

void CarouselEditor::drawBackgroundArtwork(CDrawContext* pContext) {
    CRect rect = getViewSize();
    
    // Main dark background
    pContext->setFillColor(COLOR_BG_DARK);
    pContext->fillRect(rect);
    
    // Brushed metal edge effect
    CColor metalGradient = COLOR_METAL;
    pContext->setFillColor(metalGradient);
    pContext->fillRect(CRect(rect.left, rect.top, rect.right, rect.top + 40));
    pContext->fillRect(CRect(rect.left, rect.bottom - 40, rect.right, rect.bottom));
    
    // Worn/weathered texture overlay
    pContext->setFrameColor(CColor(30, 35, 45, 100));
    for (int i = 0; i < rect.getWidth(); i += 20) {
        pContext->drawLine(CPoint(rect.left + i, rect.top), 
                          CPoint(rect.left + i + 5, rect.top + 20));
    }
    
    // "Ancient" artifact border with glow
    pContext->setFrameColor(COLOR_GLOW_CYAN);
    pContext->setLineWidth(2.0f);
    pContext->drawRect(CRect(rect.left + 2, rect.top + 2, 
                             rect.right - 2, rect.bottom - 2));
    
    // Inner sci-fi border
    pContext->setFrameColor(COLOR_GLOW_AMBER);
    pContext->setLineWidth(1.0f);
    pContext->drawRect(CRect(rect.left + 8, rect.top + 8, 
                             rect.right - 8, rect.bottom - 8));
}

void CarouselEditor::drawRetroGrid(CDrawContext* pContext) {
    CRect rect = getViewSize();
    
    pContext->setFrameColor(COLOR_GRID);
    pContext->setLineWidth(0.5f);
    
    // Vertical grid lines (scan effect)
    for (int x = (int)rect.left; x < (int)rect.right; x += 30) {
        pContext->drawLine(CPoint(x, rect.top), CPoint(x, rect.bottom));
    }
    
    // Horizontal grid lines
    for (int y = (int)rect.top; y < (int)rect.bottom; y += 30) {
        pContext->drawLine(CPoint(rect.left, y), CPoint(rect.right, y));
    }
}

void CarouselEditor::drawGlowEffect(CDrawContext* pContext, const CRect& rect,
                                    const CColor& glowColor, float intensity) {
    // Draw subtle glow at corners (lens flare aesthetic)
    CColor adjustedGlow = glowColor;
    adjustedGlow.alpha = (int)(adjustedGlow.alpha * intensity);
    
    pContext->setFillColor(adjustedGlow);
    
    // Top-left corner glow
    CRect cornerRect(rect.left, rect.top, rect.left + 100, rect.top + 80);
    pContext->fillEllipse(cornerRect);
    
    // Bottom-right corner glow
    cornerRect.offset(rect.right - rect.left - 100, rect.bottom - rect.top - 80);
    pContext->fillEllipse(cornerRect);
}

} // namespace CarouselReverb
