import discord
import os
from dotenv import load_dotenv
import starboard
import two
import random

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
    await two.on_message(message, client.user)
    await starboard.on_message(message, client)

    if message.author == client.user:
        return

    await bot_interactions(message) 

@client.event
async def on_message_edit(before, after):
    await two.on_message_edit(before, after, client)

@client.event
async def on_raw_reaction_add(payload):
    await starboard.on_reaction(payload, client)

async def bot_interactions(message):
    if "silksong" in message.content.lower():
        options = ["*sigh* bapanada.", "GESSOOOOOOOOOOO", "velmi artrid", "*sigh* apaul", "SHAW", "patamas geo", "DOMA DOMA!! DOMA DOMA DOMA!!!", "RAVA"]
        rand = random.randint(0,len(options) - 1)
        await message.channel.send(options[rand])
    if "step 3" in message.content.lower():
        await message.channel.send("SQUISH")  
    if "hazelbot" in message.content.lower():
        options = ["did someone say my name?", "hey ;]", "hello!!", ":3", "SHUT THE ###### #### ##### ###"]
        rand = random.randint(0, len(options) - 1)
        await message.channel.send(options[rand])

client.run(TOKEN)

input("Waiting for input.")
