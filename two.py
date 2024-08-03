import discord

CHANNEL_NAME = "2"

async def two_on_message(message, bot):
    
    print(f"recieved message in channel: {message.channel}")
    
    if message.author == bot:
        return
    


    if message.channel.name == CHANNEL_NAME:
        if await search_string(message.content):
            print(f"two: message {message.content} is Acceptable.")
        else:
            print(f"two: message {message.content} is Unacceptable. Deleting...")
            await message.delete()

valid_substrings = ["2", "two"]

async def search_string(string):
    for x in valid_substrings:
        if x in string:
            print(f"two: found substring {x} in string {string}!")
            return True
        else:
            print(f"two: could not find substring {x} in string {string}.")
    return False
    
