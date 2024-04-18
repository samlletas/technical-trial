-- Q2 - Fix or improve the implementation of the below method

function printSmallGuildNames(memberCount)
  local selectGuildQuery = "SELECT name FROM guilds WHERE max_members < %d;"
  local resultId = db.storeQuery(string.format(selectGuildQuery, memberCount))

  -- No need to continue if no results are found. I also thought it would be nice
  -- to let the user know that no results were found just to avoid the impression
  -- that something went wrong (better than not seeing any output at all I think).
  if not resultId then
    print(string.format("Found no guilds that have less than %d max members", memberCount))
    return
  end

  -- Loop over the found rows and print the name column, stop until no more rows remain.
  -- According to some examples I saw at the Forgotten Server source it seems that the call
  -- to the 'getString' function was missing the 'resultId' parameter so I added it back.
  repeat
    local guildName = result.getString(resultId, "name")
    print(guildName)
  until not result.next(resultId)

  -- Do cleanup after printing is completed.
  result.free(resultId)
end
