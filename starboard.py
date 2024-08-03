import discord


CLIPS_CHANNEL_ID = 1261665509531979797
TOP_CLIPS_CHANNEL_ID = 1269110989387857971

REACTION_EMOJI = "⭐"
async def starboard_on_message(message, client):
    if message.channel.id != CLIPS_CHANNEL_ID:
        return

    if await starboard_check_ttv_link(message.content):
        await message.add_reaction("⭐")
    else:
        await message.delete()


REACTIONS_REQUIRED = 3
BURST_SCORE = 2
async def starboard_on_reaction(payload, client):
    if payload.channel_id != CLIPS_CHANNEL_ID:
        return
    if payload.emoji.name != REACTION_EMOJI:
        return
    
    score = -1 # starting at -1 to undo the vote that the bot places on the message :3
    message = await client.get_channel(CLIPS_CHANNEL_ID).fetch_message(payload.message_id)
    print(f"Starboard react on message \"{message.content}\" detected!")
    for x in message.reactions:
        if payload.burst:
            # score += BURST_SCORE * x.count
            message.channel.send("i can't test super reacts yet, so they don't work currently. sorry!!")
        else:
            score += 1 * x.count

    if score >= REACTIONS_REQUIRED:
        await star_message(message, client)

async def star_message(message, client):
    print(f"starboard: Sent message \"{message.content}\" to the starboard!")
    channel = client.get_channel(TOP_CLIPS_CHANNEL_ID)
    edited_message = message.content.replace("\n", "\n > ")
    msg = f"> {edited_message}\n \- submitted by <@{message.author.id}>"

    await channel.send(msg)

TTV_LINK = "twitch.tv/hazelnutstudio/clip/"
async def starboard_check_ttv_link(message):
    if TTV_LINK in message:
        return True
    else:
        return False
