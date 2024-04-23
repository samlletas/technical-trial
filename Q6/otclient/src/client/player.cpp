/*
 * Copyright (c) 2010-2020 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Since the dash effect needs to render the player sprite multiple times I
// decided to add the code here in the Player class.

#include "player.h"
#include "shadermanager.h"
#include <framework/stdext/math.h>
#include <framework/platform/platformwindow.h>

void Player::onPositionChange(const Position& newPos, const Position& oldPos)
{
    Creature::onPositionChange(newPos, oldPos);

    // We need to know the angle to which the player is moving in order to
    // render the dash effect ghosts in the opposite direction.
    m_dashEffectAngle = Position::getAngleFromPositions(oldPos, newPos);
}

void Player::draw(const Point& dest, float scaleFactor, bool animate, LightView *lightView)
{
    // Hack: For testing purposes I'll be triggering the dash effect from here with the arrow keys.
    m_isDashing = 
        g_window.isKeyPressed(Fw::KeyUp) || 
        g_window.isKeyPressed(Fw::KeyDown) || 
        g_window.isKeyPressed(Fw::KeyLeft) || 
        g_window.isKeyPressed(Fw::KeyRight);

    drawDashEffect(dest, scaleFactor, animate, lightView);
    
    if (m_isDashing)
    {
        // Add an outline to the player sprite when dashing.
        g_painter->setShaderProgram(g_shaders.getOutlineShader());
    }

    Creature::draw(dest, scaleFactor, animate, lightView);

    // We don't want to add an outline to other sprites so let's 
    // reset back to the default shader.
    g_painter->resetShaderProgram();
}

void Player::drawDashEffect(const Point& dest, float scaleFactor, bool animate, LightView *lightView)
{
    if (m_isDashing)
    {
        // The dash effect timer controls how fast the ghosts follow the player after he stops,
        // no need to keep it running while dashing.
        m_dashEffectTimer.restart();
    }
    else if (m_dashEffectTimer.timeElapsed() > DASH_EFFECT_DURATION)
    {
        // If not dashing then cancel the effect after the timer ends.
        m_dashEffectTimer.stop();
        return;
    }

    // Smoothly move the ghosts towards the player when he stops.
    m_dashEffectDistance = stdext::lerp(DASH_EFFECT_DISTANCE_MAX, 0, m_dashEffectTimer.timeElapsed() / DASH_EFFECT_DURATION);

    for (int i = 1; i <= DASH_EFFECT_GHOSTS; i++)
    {
        // Place the ghost in the opposite direction to which the player is moving. 
        Point ghostPosition = Point(dest);
        ghostPosition.x -= (i * m_dashEffectDistance * std::cos(m_dashEffectAngle));
        ghostPosition.y += (i * m_dashEffectDistance * std::sin(m_dashEffectAngle));

        // Gradually make each ghost more transparent.
        float opacity = stdext::lerp(DASH_EFFECT_OPACITY_MAX, DASH_EFFECT_OPACITY_MIN, float(i) / DASH_EFFECT_GHOSTS);

        g_painter->setOpacity(opacity);
        Creature::draw(ghostPosition, scaleFactor, animate, lightView);
        g_painter->resetOpacity();
    }
}
