import discord
import os
from dotenv import load_dotenv
from two import two_on_message, two_on_message_edit

load_dotenv()

TOKEN = os.getenv('DISCORD_TOKEN')


intents = discord.Intents.default()
intents.message_content = True

client = discord.Client(intents=intents)

@client.event
async def on_ready():
    print(f'logged in as {client.user}!')

@client.event
async def on_message(message):
    await two_on_message(message, client.user)
    
    if message.author == client.user:
        return

    if message.content.startswith('hello'):
        await message.channel.send('Hello!')

@client.event
async def on_message_edit(before, after):
    await two_on_message_edit(before, after, client.user)

def get_botUser():
    return client.user

client.run(TOKEN)

input("Waiting for input.")
