-- The spell for this question is actually very similar to 'apocalypse.lua'
-- which helped a ton while I was figuring out how to make a custom spell.

-- Settings for the initial batch of tornados.
local initProbability = 0.7
local initDelayMin = 0
local initDelayMax = 500

-- Settings for tornados lifetime.
local maxAttempts = 3
local delayMin = 1000
local delayMax = 3000

-- I used the SpellCreator app to generate the same shape as in the question's video:
-- https://otland.net/threads/spellcreator-a-graphical-spell-creation-enviroment.160371/
local castArea = {
    { 0, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 1, 0, 1, 0, 0 },
    { 0, 1, 0, 1, 0, 1, 0 },
    { 1, 0, 1, 3, 1, 0, 1 },
    { 0, 1, 0, 1, 0, 1, 0 },
    { 0, 0, 1, 0, 1, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0 }
}

function spellCallback(cid, position, count)
    local caster = Creature(cid)

    -- Avoids casting tornados at the players's position and stops
    -- casting if the player no longer exists.
    if not caster or caster:getPosition() == position then
        return
    end

    -- Tries to randomly cast a tornado.
    if count > 0 or math.random() <= initProbability then
        doAreaCombat(cid, COMBAT_ICEDAMAGE, position, 0, 0, 0, CONST_ME_ICETORNADO)
    end

    -- Keeps trying until we run out of attempts.
    if count < maxAttempts then
        count = count + 1
        local delay = math.random(delayMin, delayMax)
        addEvent(spellCallback, delay, cid, position, count)
    end
end

function onTargetTile(creature, position)
    -- Applies a small delay to each tornado of the initial batch,
    -- looks nicer than casting them at the same time.
    local delay = math.random(initDelayMin, initDelayMax)
    addEvent(spellCallback, delay, creature:getId(), position, 0)
end

local combat = Combat()
combat:setArea(createCombatArea(castArea))
combat:setCallback(CALLBACK_PARAM_TARGETTILE, "onTargetTile")

function onCastSpell(creature, variant)
    return combat:execute(creature, variant)
end
