import discord
import re
import log
import json
import os
import random
import time

CHANNEL_NAME = "counting"
SAVE_FILE = "counting-stats.txt"
number = 1

evil = False
evil_number = 0

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
        save_contents = {'number':1, 'last_author':0, 'st_highest_count':0, 'st_failures':0, 'st_counts':0}
        number = 1 
        last_author = 0

    
async def on_message(message):
    global number
    global last_author
    global evil
    global evil_number
    if message.author == client.user:
        return
    if message.channel.name != CHANNEL_NAME:
        return

    user_stats = save_contents.get(f"st_user_{message.author.id}")
    if user_stats == None:
        user_stats = {}


    message_valid = await is_message_valid(message)
    if message_valid == 0:
        await message.add_reaction("❌")
        if save_contents['st_highest_count'] == number - 1:
            save_contents.update({'st_ruinedby':message.author.id})

        save_contents['st_failures'] += 1
        user_stats.update({"failures":user_stats.get("failures", 0) + 1})

        if user_stats.get("biggest_failure", 0) < number - 1:
            user_stats.update({"biggest_failure":number - 1})
        

        save_contents.update({f"st_user_{message.author.id}":user_stats})
        if evil and (str(evil_number + 1) in message.content):
            await message.channel.send(f"LOOK AT THIS IDIOT. THIS ABSOLUTE BUFFOON. THEY REALLY THOUGHT THEY COULD TRUST ME . WHAT A SILLY LITTLE CREATURE.")
        else:
            await message.channel.send(f"<@{message.author.id}> FAILED. THEY ARE A FAILURE. THEY SUCK. ETC.")

        number = 1
        last_author = 0

        
        
        evil = False

        await save_state()

        return

    elif message_valid == 2:
        return

    last_author = message.author.id
    number += 1

    save_contents['st_counts'] += 1
    user_stats.update({"total_counts":user_stats.get("total_counts", 0) + 1})

    if user_stats.get("highest_count", 0) <= number - 1:
        user_stats.update({"highest_count":number - 1,"highest_count_timestamp":f"{round(time.time())}"})

    await message.add_reaction("✅")

    save_contents.update({f"st_user_{message.author.id}":user_stats})

    evil = False

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
    
    if save_contents['st_highest_count'] < number - 1:
        save_contents['st_highest_count'] = number - 1
        save_contents['st_ruinedby'] = ""
        save_contents.update({'st_highest_count_timestamp':f"{round(time.time())}"})

    file_contents = json.dumps(save_contents)
    file.write(file_contents)
    await log.info(f"counting: saved state")
    file.close()

async def get_savefile():
    return save_contents

async def clast(interaction):
    global evil
    global evil_number
    if random.random() < 0.2:
        await log.info("evil hazelbot activated >:3")
        evil = True
        rand = random.randint(-5, 5)
        if rand == 0:
            rand = random.randint(-10000, 10000)
        if rand == 0:
            await interaction.response.send_message("sorry loser. i just won the lottery. bye .")
            return
        evil_number = abs((number - 1) + rand)
        await interaction.response.send_message(f"The most recent number was {evil_number}")
        return
    else:
        await interaction.response.send_message(f"The most recent number was {number - 1}")
