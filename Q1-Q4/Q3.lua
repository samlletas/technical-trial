-- Q3 - Fix or improve the name and the implementation of the below method

-- This function seems to remove a member from the player's party so I
-- renamed it accordingly, also used camelCasing style to keep consistency
-- with the function/parameter names from Q1 and Q2.
function removeMemberFromPlayerParty(playerId, memberName)
  -- Made this variable local to the function, no need to have it global.
  local player = Player(playerId)
  local party = player:getParty()

  -- It may be possible that the player is not in a party so we exit the
  -- function if that's the case. I also think only the leader should be
  -- able to remove other members so the condition validates that as well.
  if not party or party:getLeader() ~= player then
    return
  end

  -- 'k' variable was unused so I renamed it to underscore. Also renamed
  -- 'v' to 'member' to make the code a bit more readable.
  for _, member in pairs(party:getMembers()) do
    -- Instead of retrieving a member by name per iteration I changed the
    -- condition to do a name comparision, that way the member will only be
    -- retrieved once if the names match.
    if member:getName() == memberName then
      party:removeMember(Player(memberName))
    end
  end
end
