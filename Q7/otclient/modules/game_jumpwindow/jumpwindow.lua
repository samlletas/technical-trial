local jumpWindow = nil
local jumpButton = nil
local buttonSpeed = 100

function init()
    g_ui.importStyle('jumpwindow')

    -- I want the window to open as soon as the game starts, so let's connect to the
    -- corresponding events when the module loads.
    connect(g_game, { onGameStart = openWindow })
    connect(g_game, { onGameEnd = closeWindow })
end

function terminate()
    -- Let's not forget to do cleanup after the module unloads.
    disconnect(g_game, { onGameStart = openWindow })
    disconnect(g_game, { onGameEnd = closeWindow })
end

function openWindow()
    -- I need the references to the window and button in order to manipulate them.
    jumpWindow = g_ui.createWidget('JumpWindow', rootWidget)
    jumpButton = jumpWindow:getChildById('jumpButton')

    -- Prevent the button from always starting at the same position.
    resetButtonPosition()

    -- Connect to the render event to update the button's position each frame.
    connect(g_app, { onForegroundFrame = updateButtonPosition })
end

function closeWindow()
    -- Cleanup after window closes.
    if jumpWindow then
        jumpWindow:destroy()
        jumpWindow = nil
        jumpButton = nil
    end

    -- No need to update the button each frame anymore.
    disconnect(g_app, { onForegroundFrame = updateButtonPosition })
end

function resetButtonPosition()
    -- Move the button horizontally to the right side of the window.
    local newX = jumpWindow:getX() + jumpWindow:getWidth()

    -- Randomize the button's position vertically, but make sure to stay inside the window.
    local newY = math.random(jumpWindow:getY(), jumpWindow:getY() + jumpWindow:getHeight())

    setButtonPosition(newX, newY)
end

function updateButtonPosition(deltaTime)
    -- Move the button horizontally each frame, the speed is multiplied by deltaTime to
    -- achieve smooth framerate-independent movement. This way the button will cover more
    -- distance per frame at lower framerates (to make up for the lower update calls) and
    -- viceversa (cover less distance per frame at higher framerates)
    local newX = jumpButton:getX() - (buttonSpeed * deltaTime)

    -- Keep updating the button's position until it reaches the left side of the window,
    -- considers the padding of the window to prevent the button from overlapping the border.
    if newX > (jumpWindow:getX() + jumpWindow:getPaddingLeft()) then
        setButtonPosition(newX, jumpButton:getY())
    else
        resetButtonPosition()
    end
end

function setButtonPosition(x, y)
    -- If we don't break anchors then attempts to change position won't work.
    jumpButton:breakAnchors()

    -- We can change position now.
    jumpButton:setX(x)
    jumpButton:setY(y)

    -- Re-anchor the button so that it stays inside the window.
    jumpButton:bindRectToParent()
end
