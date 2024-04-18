// Q4 - Assume all method calls work fine. Fix the memory leak issue in below method

void Game::addItemToPlayer(const std::string &recipient, uint16_t itemId)
{
    Player *player = g_game.getPlayerByName(recipient);
    if (!player)
    {
        player = new Player(nullptr);
        if (!IOLoginData::loadPlayerByName(player, recipient))
        {
            // If I'm understanding correctly, this return statement here happens when a player
            // with the specified name could not be found (neither online nor offline), so in that
            // case I think we should free the allocated instance here before exiting the function.
            delete player;
            return;
        }
    }

    Item *item = Item::CreateItem(itemId);
    if (!item)
    {
        // Even if item creation fails, we should still check if we allocated a new instance for
        // an offline player and free it's memory before exiting the function.
        if (player->isOffline())
        {
            delete player;
        }

        return;
    }

    g_game.internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT);

    if (player->isOffline())
    {
        IOLoginData::savePlayer(player);

        // If the player is offline then it means that neither the player nor the item are
        // active in the current game session so memory for both should be cleared.
        delete player;
        delete item;
    }
}
