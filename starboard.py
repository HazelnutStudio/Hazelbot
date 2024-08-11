import discord
import log

CLIPS_CHANNEL_ID = 1261665509531979797
TOP_CLIPS_CHANNEL_ID = 1269110989387857971

REACTION_EMOJI = "⭐"
async def on_message(message, client):
    if message.channel.id != CLIPS_CHANNEL_ID:
        return

    if await check_ttv_link(message.content):
        await message.add_reaction("⭐")
    else:
        await message.delete()


REACTIONS_REQUIRED = 3
BURST_SCORE = 2
async def on_reaction(payload, client):
    if payload.channel_id != CLIPS_CHANNEL_ID:
        return
    if payload.emoji.name != REACTION_EMOJI:
        return
    
    score = -1 # starting at -1 to undo the vote that the bot places on the message :3
    message = await client.get_channel(CLIPS_CHANNEL_ID).fetch_message(payload.message_id)
    await log.info(f"starboard: Starboard react on message \"{message.content}\" detected!")
    for x in message.reactions:
        if payload.burst:
            # score += BURST_SCORE * x.count
            await log.info("starboard: super react detected. but they DON'T WORK!! :(")
            message.channel.send("i can't test super reacts yet, so they don't work currently. sorry!!")
        else:
            score += 1 * x.count

    if score >= REACTIONS_REQUIRED:
        await star_message(message, client)
        return True
    else:
        return False

async def star_message(message, client):
    await log.info(f"starboard: Sent message \"{message.content}\" to the starboard!")
    channel = client.get_channel(TOP_CLIPS_CHANNEL_ID)
    edited_message = message.content.replace("\n", "\n > ")
    msg = f"> {edited_message}\n \- submitted by <@{message.author.id}>"

    await channel.send(msg)
    await message.delete()

VALID_LINKS = ["twitch.tv/hazelnutstudio/clip", "clips.twitch.tv/"]
async def check_ttv_link(message):
    for x in VALID_LINKS:
        if x in message:
            return True
    return False
