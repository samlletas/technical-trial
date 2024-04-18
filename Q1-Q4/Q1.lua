-- Q1 - Fix or improve the implementation of the below methods

-- Added this variable to make it a bit more clear what 1000 means, and made it
-- local to protect it from modifications from external lua scripts.
-- For the purposes of this trial I named it just 'storageId' since I don't
-- know what 1000 maps to (Is it item count? Quest progress? A state switch?).
local storageId = 1000

-- The hardcoded 1000 in the addEvent call initially confused me since I thought it
-- was related to the storageId but after looking at the Forgotten Server source I
-- realized that this parameter is actually a delay time for triggerring the event.
local delayTime = 1000

-- This callback function doesn't seem to need changes, if the intent was to
-- reset an item count or quest progress then I would set the value to 0 but
-- I'm leaving it unmodified since -1 could be a state switch.
local function releaseStorage(player)
  player:setStorageValue(storageId, -1)
end

function onLogout(player)
  -- If we were resetting item count or quest progress I would change this
  -- condition to '> 0' but I'm leaving it unmodified since it could be checking
  -- for a specific state mapped to 1.
  if player:getStorageValue(storageId) == 1 then
    addEvent(releaseStorage, delayTime, player)
  end
  return true
end
