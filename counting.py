import discord
import re
import log
import json

CHANNEL_NAME = "counting"
SAVE_FILE = "counting-stats.txt"
number = 1

async def initialize(_client):
    global client
    global number
    global last_author
    global save_contents

    client = _client
    
    if os.path.isfile(SAVE_FILE):
        file = open(SAVE_FILE, "r")
        save_contents = json.loads(file.read())
        number = save_contents['number']
        last_author = save_contents['last_author']
    else:
        save_contents = {'number':0, 'last_author':0}
        number = 0
        last_author = 0

    
async def on_message(message):
    global number
    global last_author
    if message.author == client.user:
        return
    if message.channel.name != CHANNEL_NAME:
        return

    message_valid = await is_message_valid(message)
    if message_valid == 0:
        await message.add_reaction("❌")
        number = 1
        last_author = 0
        await message.channel.send(f"<@{message.author.id}> FAILED. THEY ARE A FAILURE. THEY SUCK. ETC.")
        return
    elif message_valid == 2:
        return

    last_author = message.author.id
    
    number += 1
    await message.add_reaction("✅")
    await save_state()

async def on_message_edit(before, after):
    if before.channel.name != CHANNEL_NAME:
        return
    await before.channel.send("idc. shut up.")
    await log.info("counting: message edited")

async def is_message_valid(message):
    # credit: @benetsugarboy for the REGEX (I HATE REGEX!!!!)

    match = re.search("([0-9]+)(\.[0-9]+)?", message.content)
    if match == None:
        return 2    
    num = round(float(match.group()))

    if num != number:
        return 0

    if last_author == message.author.id:
        return 0
    return 1

async def save_state():
    file = open(SAVE_FILE, "w")

    save_contents['number'] = number
    save_contents['last_author'] = last_author

    file_contents = json.dumps(file_contents)
    file.write(file_contents)
    await log.info(f"counting: saved state\n```\n{file_contents}\n```")
    file.close()
