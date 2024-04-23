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

#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

// @bindclass
class Player : public Creature
{
public:
    Player() { }
    virtual ~Player() { }

    PlayerPtr asPlayer() { return static_self_cast<Player>(); }
    bool isPlayer() { return true; }

    virtual void onPositionChange(const Position& newPos, const Position& oldPos);
    virtual void draw(const Point& dest, float scaleFactor, bool animate, LightView *lightView = nullptr);

private:
    virtual void drawDashEffect(const Point& dest, float scaleFactor, bool animate, LightView *lightView = nullptr);
    bool m_isDashing;
    float m_dashEffectDistance;
    float m_dashEffectAngle;
    Timer m_dashEffectTimer;
};

const int DASH_EFFECT_GHOSTS = 4;
const float DASH_EFFECT_DISTANCE_MAX = 24;
const float DASH_EFFECT_DURATION = 0.1f;
const float DASH_EFFECT_OPACITY_MIN = 0.2f;
const float DASH_EFFECT_OPACITY_MAX = 0.8f;

#endif
