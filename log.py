import discord

CHANNEL_ID = 1255572679386202147
async def initialize(_client):
    global client
    global channel
    client = _client
    channel = client.get_channel(CHANNEL_ID)

async def info(message):
    print(message)
    await channel.send(message)
