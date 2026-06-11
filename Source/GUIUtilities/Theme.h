//
//  Theme.h
//  KissOfShame (Rev 2)
//
//  The two faces of the machine:
//
//    HERITAGE — the original 2014 skeuomorphic identity: Yannick Bonnefoy's
//    filmstrip knobs, bitmap VU meters, the photographed faceplate. Untouched.
//
//    MODERN — the same instrument re-drawn in the flat, *dimensional*
//    language of recent macOS. Flat is a philosophy, not an excuse: every
//    surface here carries light — machined bezels, inner shadows, specular
//    sweeps, surface grain, and the signature pink blooming like a lamp,
//    not a fill. Fully vector, so it renders pixel-perfect at any retina
//    scale.
//
//  Components never decide colors themselves; they branch on UIEra and call
//  these helpers, so each era stays coherent everywhere.
//

#pragma once

#include "../shameConfig.h"

enum class UIEra
{
    heritage,
    modern
};

namespace ModernTheme
{
    // Palette: macOS-dark materials + the signature pink of the cross logo.
    inline const Colour panel        { 0xff1c1c1e };
    inline const Colour panelRaised  { 0xff2c2c2e };
    inline const Colour panelDeep    { 0xff111113 };
    inline const Colour outline      { 0xff3a3a3c };
    inline const Colour textPrimary  { 0xffe8e8ea };
    inline const Colour textDim      { 0xff98989d };
    inline const Colour accent       { Colour::fromFloatRGBA(1.0f, 0.216f, 0.384f, 1.0f) };
    inline const Colour accentHot    { Colour::fromFloatRGBA(1.0f, 0.10f, 0.16f, 1.0f) };
    inline const Colour vuCream      { 0xfff0e2c4 };

    inline constexpr float rotaryStart = -2.356f; // -135 degrees from 12 o'clock
    inline constexpr float rotaryEnd   =  2.356f;

    inline Font labelFont(float height)
    {
        return Font(FontOptions().withHeight(height)).boldened().withExtraKerningFactor(0.06f);
    }

    //==========================================================================
    // Surface grain: a cached noise tile, laid over panels at low opacity so
    // large dark areas read as material instead of dead fill.
    inline const Image& grainTile()
    {
        static const Image tile = []
        {
            Image img(Image::ARGB, 128, 128, true);
            Random r(0x51043); // deterministic
            for (int y = 0; y < 128; ++y)
                for (int x = 0; x < 128; ++x)
                {
                    const float v = r.nextFloat();
                    if (v > 0.5f)
                        img.setPixelAt(x, y, Colours::white.withAlpha((v - 0.5f) * 0.10f));
                    else
                        img.setPixelAt(x, y, Colours::black.withAlpha((0.5f - v) * 0.12f));
                }
            return img;
        }();
        return tile;
    }

    inline void fillGrain(Graphics& g, juce::Rectangle<float> r, float opacity = 1.0f)
    {
        g.setTiledImageFill(grainTile(), 0, 0, opacity);
        g.fillRect(r);
    }

    //==========================================================================
    // A machined screw head — the corners of anything that claims to be a panel.
    inline void drawScrew(Graphics& g, Point<float> c, float radius, float slotAngle)
    {
        g.setColour(Colours::black.withAlpha(0.45f));
        g.fillEllipse(c.x - radius, c.y - radius + 1.2f, radius * 2, radius * 2);

        ColourGradient head(Colour(0xff47474b), c.x, c.y - radius,
                            Colour(0xff202022), c.x, c.y + radius, false);
        g.setGradientFill(head);
        g.fillEllipse(c.x - radius, c.y - radius, radius * 2, radius * 2);

        g.setColour(Colours::white.withAlpha(0.10f));
        g.drawEllipse(c.x - radius, c.y - radius, radius * 2, radius * 2, 0.8f);

        const auto p1 = c.getPointOnCircumference(radius * 0.72f, slotAngle);
        const auto p2 = c.getPointOnCircumference(radius * 0.72f, slotAngle + MathConstants<float>::pi);
        g.setColour(Colours::black.withAlpha(0.7f));
        g.drawLine({ p1, p2 }, 1.4f);
    }

    // Soft multi-pass drop shadow under a circle.
    inline void dropShadowEllipse(Graphics& g, juce::Rectangle<float> r, float strength = 0.5f)
    {
        for (int i = 3; i >= 1; --i)
        {
            g.setColour(Colours::black.withAlpha(strength * 0.16f * (float) i));
            g.fillEllipse(r.translated(0.0f, 2.0f + (float) i).expanded((float) (4 - i)));
        }
    }

    // Accent arc with bloom: a wide soft pass under a crisp core, ending in
    // a lit dot — pink as a lamp, not a line.
    inline void drawBloomArc(Graphics& g, Point<float> centre, float radius,
                             float fromAngle, float toAngle, Colour colour, float coreWidth = 2.8f)
    {
        Path arc;
        arc.addCentredArc(centre.x, centre.y, radius, radius, 0.0f, fromAngle, toAngle, true);

        g.setColour(colour.withAlpha(0.18f));
        g.strokePath(arc, PathStrokeType(coreWidth * 3.2f, PathStrokeType::curved, PathStrokeType::rounded));
        g.setColour(colour.withAlpha(0.45f));
        g.strokePath(arc, PathStrokeType(coreWidth * 1.8f, PathStrokeType::curved, PathStrokeType::rounded));
        g.setColour(colour);
        g.strokePath(arc, PathStrokeType(coreWidth, PathStrokeType::curved, PathStrokeType::rounded));

        const auto tip = centre.getPointOnCircumference(radius, toAngle);
        g.setColour(colour.withAlpha(0.30f));
        g.fillEllipse(tip.x - coreWidth * 1.8f, tip.y - coreWidth * 1.8f, coreWidth * 3.6f, coreWidth * 3.6f);
        g.setColour(Colours::white.withAlpha(0.85f));
        g.fillEllipse(tip.x - coreWidth * 0.55f, tip.y - coreWidth * 0.55f, coreWidth * 1.1f, coreWidth * 1.1f);
    }

    //==========================================================================
    inline void drawKnob(Graphics& g, juce::Rectangle<float> bounds, float value01,
                         bool drawCross = false, bool extreme = false, bool hover = false)
    {
        auto r = bounds.reduced(bounds.getWidth() * 0.12f);
        const auto centre = r.getCentre();
        const float radius = r.getWidth() * 0.5f;
        const float angle = rotaryStart + value01 * (rotaryEnd - rotaryStart);
        const Colour glowColour = extreme ? accentHot : accent;

        // seated in the panel: shadow first
        dropShadowEllipse(g, r, hover ? 0.65f : 0.5f);

        if (extreme)
        {
            ColourGradient heat(accentHot.withAlpha(0.35f), centre.x, centre.y,
                                accentHot.withAlpha(0.0f), centre.x, centre.y - radius * 1.7f, true);
            g.setGradientFill(heat);
            g.fillEllipse(bounds.expanded(8.0f));
        }

        // bezel ring
        ColourGradient bezel(Colour(0xff414146), centre.x, r.getY(),
                             Colour(0xff141416), centre.x, r.getBottom(), false);
        g.setGradientFill(bezel);
        g.fillEllipse(r);

        // cap, slightly inset, with its own light
        auto cap = r.reduced(radius * 0.10f);
        ColourGradient capFill(Colour(hover ? 0xff343438 : 0xff2e2e32), centre.x, cap.getY(),
                               Colour(0xff18181a), centre.x, cap.getBottom(), false);
        g.setGradientFill(capFill);
        g.fillEllipse(cap);

        // specular sweep across the upper cap
        {
            Graphics::ScopedSaveState save(g);
            Path clip; clip.addEllipse(cap);
            g.reduceClipRegion(clip);
            ColourGradient spec(Colours::white.withAlpha(hover ? 0.10f : 0.07f),
                                centre.x - radius * 0.5f, cap.getY(),
                                Colours::transparentWhite, centre.x, centre.y, false);
            g.setGradientFill(spec);
            g.fillEllipse(cap.withHeight(cap.getHeight() * 0.55f));
        }

        // inner shadow at the cap's top seat
        g.setColour(Colours::black.withAlpha(0.35f));
        g.drawEllipse(cap.expanded(0.6f), 1.2f);
        g.setColour(Colours::white.withAlpha(0.06f));
        g.drawEllipse(r.reduced(0.6f), 1.0f);

        // tick ring
        const float tickRadius = radius + bounds.getWidth() * 0.085f;
        for (int t = 0; t <= 10; ++t)
        {
            const float a = rotaryStart + (rotaryEnd - rotaryStart) * (float) t / 10.0f;
            const bool major = (t == 0 || t == 5 || t == 10);
            const auto p1 = centre.getPointOnCircumference(tickRadius, a);
            const auto p2 = centre.getPointOnCircumference(tickRadius - (major ? 4.5f : 2.5f), a);
            g.setColour(Colours::white.withAlpha(major ? 0.28f : 0.14f));
            g.drawLine({ p1, p2 }, major ? 1.6f : 1.0f);
        }

        // track + blooming value arc
        const float arcRadius = radius + bounds.getWidth() * 0.045f;
        Path track;
        track.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f, rotaryStart, rotaryEnd, true);
        g.setColour(Colours::black.withAlpha(0.55f));
        g.strokePath(track, PathStrokeType(3.4f, PathStrokeType::curved, PathStrokeType::rounded));
        g.setColour(Colours::white.withAlpha(0.07f));
        g.strokePath(track, PathStrokeType(1.2f, PathStrokeType::curved, PathStrokeType::rounded));

        if (value01 > 0.001f)
            drawBloomArc(g, centre, arcRadius, rotaryStart, angle, glowColour, 2.6f);

        // pointer: lit bar with a shadow under it
        {
            Path pointerShadow;
            pointerShadow.addRoundedRectangle(-1.8f, -radius * 0.88f, 3.6f, radius * 0.34f, 1.8f);
            g.setColour(Colours::black.withAlpha(0.5f));
            g.fillPath(pointerShadow, AffineTransform::rotation(angle).translated(centre.x, centre.y + 1.5f));

            Path pointer;
            pointer.addRoundedRectangle(-1.5f, -radius * 0.90f, 3.0f, radius * 0.32f, 1.5f);
            g.setColour(textPrimary);
            g.fillPath(pointer, AffineTransform::rotation(angle).translated(centre.x, centre.y));

            const auto tip = centre.getPointOnCircumference(radius * 0.80f, angle);
            g.setColour(glowColour.withAlpha(0.55f));
            g.fillEllipse(tip.x - 2.2f, tip.y - 2.2f, 4.4f, 4.4f);
        }

        // the cross — brand continuity for knobs that carry it in vector form
        if (drawCross)
        {
            const float arm = radius * 0.40f;
            const float thick = jmax(3.0f, radius * 0.13f);
            g.setColour(glowColour.withAlpha(0.35f));
            g.fillRoundedRectangle(centre.x - thick * 0.7f, centre.y - arm - 1.0f, thick * 1.4f, arm * 2.0f + 2.0f, thick * 0.5f);
            g.setColour(glowColour);
            g.fillRoundedRectangle(centre.x - thick * 0.5f, centre.y - arm, thick, arm * 2.0f, thick * 0.4f);
            g.fillRoundedRectangle(centre.x - arm, centre.y - thick * 0.5f, arm * 2.0f, thick, thick * 0.4f);
        }
    }

    //==========================================================================
    // The Infernal Love mark: the inverted cross with the heart at its
    // intersection. Vector form of the brand, for wordmarks and accents —
    // the Shame knob itself uses the original 2014 artwork.
    inline void drawInfernalLoveMark(Graphics& g, juce::Rectangle<float> r,
                                     Colour crossColour, Colour heartCutout)
    {
        const float w = r.getWidth(), h = r.getHeight();
        const float cx = r.getCentreX();
        const float t = w * 0.30f;                    // bar thickness
        const float crossbarY = r.getY() + h * 0.62f; // inverted: bar in the lower third

        Path cross;
        cross.addRoundedRectangle(cx - t * 0.5f, r.getY(), t, h, t * 0.35f);
        cross.addRoundedRectangle(r.getX(), crossbarY - t * 0.5f, w, t, t * 0.35f);

        // halo
        g.setColour(crossColour.withAlpha(0.30f));
        g.strokePath(cross, PathStrokeType(2.6f));
        g.setColour(crossColour);
        g.fillPath(cross);

        // heart, cut into the intersection
        const float s = w * 0.52f;
        const float hy = crossbarY;
        Path heart;
        heart.startNewSubPath(cx, hy + 0.38f * s);
        heart.cubicTo(cx - 0.58f * s, hy + 0.02f * s, cx - 0.46f * s, hy - 0.40f * s, cx, hy - 0.12f * s);
        heart.cubicTo(cx + 0.46f * s, hy - 0.40f * s, cx + 0.58f * s, hy + 0.02f * s, cx, hy + 0.38f * s);
        heart.closeSubPath();
        g.setColour(heartCutout);
        g.fillPath(heart);
    }

    //==========================================================================
    // Recessed seats: controls sit IN the deck, not on it.
    inline void drawRoundWell(Graphics& g, juce::Rectangle<float> r)
    {
        g.setColour(Colours::black.withAlpha(0.38f));
        g.fillEllipse(r);
        g.setColour(Colours::black.withAlpha(0.55f));
        g.drawEllipse(r.translated(0.0f, -0.8f), 1.6f);
        g.setColour(Colours::white.withAlpha(0.07f));
        g.drawEllipse(r.translated(0.0f, 0.9f), 1.0f);
    }

    inline void drawRectWell(Graphics& g, juce::Rectangle<float> r, float corner)
    {
        g.setColour(Colours::black.withAlpha(0.38f));
        g.fillRoundedRectangle(r, corner);
        g.setColour(Colours::black.withAlpha(0.55f));
        g.drawRoundedRectangle(r.translated(0.0f, -0.8f), corner, 1.6f);
        g.setColour(Colours::white.withAlpha(0.07f));
        g.drawRoundedRectangle(r.translated(0.0f, 0.9f), corner, 1.0f);
    }

    //==========================================================================
    // Ember-glass VU: smoked dark glass over a pink-amber lamp that breathes
    // with the program level. The needle is ivory with a pink bloom trail.
    inline void drawVUMeter(Graphics& g, juce::Rectangle<float> bounds, float value01, float glow01 = 0.0f)
    {
        auto r = bounds.reduced(2.0f);
        glow01 = jlimit(0.0f, 1.0f, glow01);
        const Colour emberLamp = Colour(0xffff6a4e).interpolatedWith(accent, 0.55f);
        const Colour ivory = Colour(0xffe9dcc0);

        // housing shadow + machined bezel
        g.setColour(Colours::black.withAlpha(0.5f));
        g.fillRoundedRectangle(r.translated(0, 2.0f).expanded(1.5f), 9.0f);

        ColourGradient bezel(Colour(0xff424247), r.getCentreX(), r.getY(),
                             Colour(0xff101012), r.getCentreX(), r.getBottom(), false);
        g.setGradientFill(bezel);
        g.fillRoundedRectangle(r, 9.0f);

        // smoked glass face
        auto face = r.reduced(4.0f);
        ColourGradient smoke(Colour(0xff221511), face.getCentreX(), face.getY(),
                             Colour(0xff0c0807), face.getCentreX(), face.getBottom(), false);
        g.setGradientFill(smoke);
        g.fillRoundedRectangle(face, 6.0f);

        const auto pivot = Point<float>(face.getCentreX(), face.getBottom() - face.getHeight() * 0.16f);
        const float needleLen = face.getHeight() * 0.62f;
        const float minAngle = -0.85f, maxAngle = 0.85f;

        // the lamp behind the glass: brightness rides the audio
        {
            Graphics::ScopedSaveState save(g);
            Path clip; clip.addRoundedRectangle(face, 6.0f);
            g.reduceClipRegion(clip);

            ColourGradient lamp(emberLamp.withAlpha(0.16f + 0.45f * glow01), pivot.x, pivot.y,
                                emberLamp.withAlpha(0.0f), pivot.x, face.getY() - face.getHeight() * 0.25f, true);
            g.setGradientFill(lamp);
            g.fillRoundedRectangle(face, 6.0f);

            fillGrain(g, face, 0.35f);
        }

        // scale: warm ivory ticks, brighter as they approach the red zone
        for (int t = 0; t <= 12; ++t)
        {
            const float frac = (float) t / 12.0f;
            const float a = minAngle + (maxAngle - minAngle) * frac;
            const bool major = (t % 3 == 0);
            const auto p1 = pivot.getPointOnCircumference(needleLen, a);
            const auto p2 = pivot.getPointOnCircumference(needleLen - (major ? 6.0f : 3.2f), a);
            const Colour tickColour = frac > 0.78f ? accent : ivory;
            g.setColour(tickColour.withAlpha((major ? 0.85f : 0.45f) * (0.55f + 0.45f * glow01)));
            g.drawLine({ p1, p2 }, major ? 1.5f : 0.9f);
        }

        // red zone wedge, glowing
        {
            Path red;
            const float a0 = minAngle + (maxAngle - minAngle) * 0.78f;
            red.addCentredArc(pivot.x, pivot.y, needleLen + 1.5f, needleLen + 1.5f, 0.0f, a0, maxAngle, true);
            g.setColour(accent.withAlpha(0.30f + 0.35f * glow01));
            g.strokePath(red, PathStrokeType(4.6f, PathStrokeType::curved, PathStrokeType::butt));
            g.setColour(accent);
            g.strokePath(red, PathStrokeType(2.2f, PathStrokeType::curved, PathStrokeType::butt));
        }

        g.setColour(ivory.withAlpha(0.35f + 0.30f * glow01));
        g.setFont(labelFont(9.0f));
        g.drawText("VU", face.withTrimmedBottom(face.getHeight() * 0.30f), Justification::centred, false);

        // needle: pink bloom trail under an ivory blade
        const float needleAngle = minAngle + (maxAngle - minAngle) * jlimit(0.0f, 1.0f, value01);
        const auto tip = pivot.getPointOnCircumference(needleLen - 2.0f, needleAngle);
        g.setColour(accent.withAlpha(0.20f + 0.30f * glow01));
        g.drawLine(Line<float>(pivot, tip), 4.6f);
        g.setColour(Colours::black.withAlpha(0.35f));
        g.drawLine(Line<float>(pivot.translated(0.8f, 1.2f),
                               pivot.translated(0.8f, 1.2f).getPointOnCircumference(needleLen - 2.0f, needleAngle)), 1.8f);
        g.setColour(ivory);
        g.drawLine(Line<float>(pivot, tip), 1.6f);
        g.setColour(Colours::white.withAlpha(0.7f));
        g.fillEllipse(tip.x - 1.4f, tip.y - 1.4f, 2.8f, 2.8f);

        // pivot boss
        ColourGradient boss(Colour(0xff6d6d72), pivot.x, pivot.y - 4.0f, Colour(0xff222226), pivot.x, pivot.y + 4.0f, false);
        g.setGradientFill(boss);
        g.fillEllipse(pivot.x - 4.0f, pivot.y - 4.0f, 8.0f, 8.0f);
        g.setColour(accent.withAlpha(0.5f));
        g.drawEllipse(pivot.x - 4.0f, pivot.y - 4.0f, 8.0f, 8.0f, 0.8f);

        // glass: diagonal highlight
        {
            Graphics::ScopedSaveState save(g);
            Path clip; clip.addRoundedRectangle(face, 6.0f);
            g.reduceClipRegion(clip);
            ColourGradient glass(Colours::white.withAlpha(0.10f), face.getX(), face.getY(),
                                 Colours::transparentWhite, face.getX() + face.getWidth() * 0.55f,
                                 face.getY() + face.getHeight() * 0.7f, false);
            g.setGradientFill(glass);
            g.fillRect(face.withTrimmedBottom(face.getHeight() * 0.45f));
        }

        // bezel inner line + corner screws
        g.setColour(Colours::white.withAlpha(0.08f));
        g.drawRoundedRectangle(r.reduced(0.7f), 8.0f, 1.0f);
        drawScrew(g, { r.getX() + 6.5f, r.getY() + 6.5f }, 2.6f, 0.6f);
        drawScrew(g, { r.getRight() - 6.5f, r.getY() + 6.5f }, 2.6f, 2.2f);
        drawScrew(g, { r.getX() + 6.5f, r.getBottom() - 6.5f }, 2.6f, 1.1f);
        drawScrew(g, { r.getRight() - 6.5f, r.getBottom() - 6.5f }, 2.6f, 2.9f);
    }

    //==========================================================================
    // A large-format NAB reel — the 10.5" machined-aluminium flange of a
    // Studer/Ampex transport, not a cassette hub: three wide kidney windows
    // showing the tape pack behind, narrow spokes between them, and the
    // trilobe NAB hub adapter at the centre. packAmount sets how much tape
    // is wound on (supply reel runs fuller than take-up).
    inline void drawReel(Graphics& g, Point<float> centre, float radius, float angleRad,
                         float packAmount = 0.8f)
    {
        const float R = radius;
        auto discBounds = juce::Rectangle<float>(centre.x - R, centre.y - R, R * 2, R * 2);

        // floor shadow
        dropShadowEllipse(g, discBounds, 0.7f);

        // ------- behind the flange: backplate + wound tape pack
        g.setColour(Colour(0xff070708));
        g.fillEllipse(discBounds.reduced(R * 0.04f));

        const float packR = R * jmap(jlimit(0.0f, 1.0f, packAmount), 0.36f, 0.90f);
        ColourGradient pack(Colour(0xff1b1410), centre.x, centre.y - packR,
                            Colour(0xff0c0908), centre.x, centre.y + packR, false);
        g.setGradientFill(pack);
        g.fillEllipse(centre.x - packR, centre.y - packR, packR * 2, packR * 2);

        for (int i = 1; i <= 6; ++i)        // wind grooves
        {
            const float gr = packR * (0.42f + 0.095f * (float) i);
            g.setColour(Colours::white.withAlpha(0.025f + 0.005f * (float) i));
            g.drawEllipse(centre.x - gr, centre.y - gr, gr * 2, gr * 2, 0.7f);
        }
        g.setColour(Colour(0xff241a13).brighter(0.2f).withAlpha(0.5f)); // pack edge catch-light
        g.drawEllipse(centre.x - packR, centre.y - packR, packR * 2, packR * 2, 1.0f);

        // ------- the flange: one path, outer disc minus three kidney windows
        Path flange;
        flange.setUsingNonZeroWinding(false);
        flange.addEllipse(discBounds.reduced(R * 0.015f));

        const float winOuter = R * 0.86f, winInner = R * 0.38f;
        const float halfSpan = 0.92f; // ~53 deg each side: wide kidneys, narrow spokes
        for (int s = 0; s < 3; ++s)
        {
            const float a = angleRad + (float) s * MathConstants<float>::twoPi / 3.0f;
            Path window;
            window.addCentredArc(centre.x, centre.y, winOuter, winOuter, a, -halfSpan, halfSpan, true);
            window.addCentredArc(centre.x, centre.y, winInner, winInner, a, halfSpan, -halfSpan, false);
            window.closeSubPath();
            flange.addPath(window);
        }

        {
            Graphics::ScopedSaveState save(g);
            g.reduceClipRegion(flange);

            // brushed gunmetal, lighter where the light falls
            ColourGradient metal(Colour(0xff45454c), centre.x, centre.y - R,
                                 Colour(0xff17171a), centre.x, centre.y + R, false);
            g.setGradientFill(metal);
            g.fillEllipse(discBounds);

            // circumferential machining lines
            for (int i = 0; i < 7; ++i)
            {
                const float rr = R * (0.42f + 0.085f * (float) i);
                g.setColour(Colours::black.withAlpha(0.16f));
                g.drawEllipse(centre.x - rr, centre.y - rr, rr * 2, rr * 2, 0.6f);
            }

            // angled specular sweep
            ColourGradient spec(Colours::white.withAlpha(0.085f), centre.x - R * 0.7f, centre.y - R,
                                Colours::transparentWhite, centre.x + R * 0.2f, centre.y, false);
            g.setGradientFill(spec);
            g.fillRect(discBounds.withHeight(R * 1.1f));
        }

        // window bevels: dark cut edge + low catch-light
        for (int s = 0; s < 3; ++s)
        {
            const float a = angleRad + (float) s * MathConstants<float>::twoPi / 3.0f;
            Path window;
            window.addCentredArc(centre.x, centre.y, winOuter, winOuter, a, -halfSpan, halfSpan, true);
            window.addCentredArc(centre.x, centre.y, winInner, winInner, a, halfSpan, -halfSpan, false);
            window.closeSubPath();
            g.setColour(Colours::black.withAlpha(0.65f));
            g.strokePath(window, PathStrokeType(1.4f));
            g.setColour(Colours::white.withAlpha(0.07f));
            g.strokePath(window, PathStrokeType(0.7f), AffineTransform::translation(0.0f, 1.0f));
        }

        // machined rim band
        g.setColour(Colours::white.withAlpha(0.14f));
        g.drawEllipse(discBounds.reduced(R * 0.015f), 1.6f);
        g.setColour(Colours::black.withAlpha(0.65f));
        g.drawEllipse(discBounds, 1.2f);
        g.setColour(Colours::black.withAlpha(0.30f));
        g.drawEllipse(discBounds.reduced(R * 0.055f), 1.0f);

        // ------- NAB trilobe hub adapter, rotating with the reel
        const float hubR = R * 0.30f;
        {
            Path trilobe;
            trilobe.addEllipse(centre.x - hubR * 0.78f, centre.y - hubR * 0.78f, hubR * 1.56f, hubR * 1.56f);
            for (int l = 0; l < 3; ++l)
            {
                const float a = angleRad + (float) l * MathConstants<float>::twoPi / 3.0f
                              + MathConstants<float>::pi / 3.0f;
                const auto lc = centre.getPointOnCircumference(hubR * 0.62f, a);
                trilobe.addEllipse(lc.x - hubR * 0.42f, lc.y - hubR * 0.42f, hubR * 0.84f, hubR * 0.84f);
            }

            g.setColour(Colours::black.withAlpha(0.5f));
            g.fillPath(trilobe, AffineTransform::translation(0.0f, 1.5f));

            ColourGradient hubMetal(Colour(0xff5a5a61), centre.x, centre.y - hubR,
                                    Colour(0xff222226), centre.x, centre.y + hubR, false);
            g.setGradientFill(hubMetal);
            g.fillPath(trilobe);
            g.setColour(Colours::white.withAlpha(0.12f));
            g.strokePath(trilobe, PathStrokeType(0.9f));
        }

        // hub face + the brand's slim light
        ColourGradient hubFace(Colour(0xff37373c), centre.x, centre.y - hubR * 0.6f,
                               Colour(0xff141416), centre.x, centre.y + hubR * 0.6f, false);
        g.setGradientFill(hubFace);
        g.fillEllipse(centre.x - hubR * 0.6f, centre.y - hubR * 0.6f, hubR * 1.2f, hubR * 1.2f);
        g.setColour(accent.withAlpha(0.55f));
        g.drawEllipse(centre.x - hubR * 0.6f, centre.y - hubR * 0.6f, hubR * 1.2f, hubR * 1.2f, 1.2f);

        // spindle + drive screws
        g.setColour(Colour(0xff0c0c0d));
        g.fillEllipse(centre.x - hubR * 0.16f, centre.y - hubR * 0.16f, hubR * 0.32f, hubR * 0.32f);
        for (int b = 0; b < 3; ++b)
        {
            const float a = angleRad + (float) b * MathConstants<float>::twoPi / 3.0f;
            const auto p = centre.getPointOnCircumference(hubR * 0.40f, a);
            drawScrew(g, p, jmax(1.6f, hubR * 0.085f), a);
        }
    }

    //==========================================================================
    inline void drawButton(Graphics& g, juce::Rectangle<float> bounds, const String& label,
                           bool on, bool pressed, bool hover = false)
    {
        auto r = bounds.reduced(1.5f);
        const float corner = jmin(7.0f, r.getHeight() * 0.28f);

        if (! pressed)
        {
            g.setColour(Colours::black.withAlpha(0.40f));
            g.fillRoundedRectangle(r.translated(0, 1.5f), corner);
        }

        if (on)
        {
            // lit: accent gradient + outer bloom
            g.setColour(accent.withAlpha(hover ? 0.40f : 0.28f));
            g.fillRoundedRectangle(r.expanded(2.5f), corner + 2.5f);

            ColourGradient lit(accent.brighter(pressed ? -0.1f : 0.12f), r.getCentreX(), r.getY(),
                               accent.darker(pressed ? 0.45f : 0.25f), r.getCentreX(), r.getBottom(), false);
            g.setGradientFill(lit);
            g.fillRoundedRectangle(r, corner);

            g.setColour(Colours::white.withAlpha(0.25f));
            g.drawLine(r.getX() + corner, r.getY() + 1.0f, r.getRight() - corner, r.getY() + 1.0f, 1.0f);

            g.setColour(Colour(0xff2a060f));
        }
        else
        {
            ColourGradient body(Colour(pressed ? 0xff1a1a1c : (hover ? 0xff39393d : 0xff313135)),
                                r.getCentreX(), r.getY(),
                                Colour(pressed ? 0xff111113 : 0xff1d1d20), r.getCentreX(), r.getBottom(), false);
            g.setGradientFill(body);
            g.fillRoundedRectangle(r, corner);

            g.setColour(Colours::white.withAlpha(hover ? 0.14f : 0.08f));
            g.drawLine(r.getX() + corner, r.getY() + 1.0f, r.getRight() - corner, r.getY() + 1.0f, 1.0f);
            g.setColour(outline);
            g.drawRoundedRectangle(r, corner, 1.1f);

            g.setColour(hover ? textPrimary : textDim);
        }

        g.setFont(labelFont(jmin(11.0f, r.getHeight() * 0.42f)));
        g.drawText(label, pressed ? r.translated(0, 1.0f) : r, Justification::centred, false);
    }
}
