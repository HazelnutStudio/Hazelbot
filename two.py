import discord
from datetime import timedelta

CHANNEL_NAME = "2"

async def two_on_message(message, bot):
    
    if message.author == bot:
        return

    if message.channel.name != CHANNEL_NAME:
        return

    print(f"two: recieved message in channel")

    if await search_string(message.content):
            print(f"two: message {message.content} is Acceptable.")
    else:
        print(f"two: message {message.content} is Unacceptable. Deleting...")
        await message.delete()

async def two_on_message_edit(before, after, bot):
    if before.author == bot:
        return

    if before.channel.name != CHANNEL_NAME:
        return
    

    print(f"recieved message edit")
    editValid = await search_string(after.content)
    if editValid:
        print(f"message {before.content} edit is valid.")
    else:
        await before.channel.send(f"<@{before.author.id}> has made an invalid edit!! they will be oblitworated.")
        await before.author.edit(nick = "LOOK AT THIS [LOSER]!!")
        # before.author.nick = "THIS [LOSER] TRIED TO EDIT A MESSAGE IN #2!!"
        await before.author.timeout(timedelta(seconds=30))



valid_substrings = ["2", "two"]

async def search_string(string):
    for x in valid_substrings:
        if x in string:
            print(f"two: found substring {x} in string {string}!")
            return True
        else:
            print(f"two: could not find substring {x} in string {string}.")
    return False
    
