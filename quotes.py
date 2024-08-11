import discord
import log
import re
import datetime

async def initialize(_client):
    global client
    client = _client

async def start_vote(message):
    await log.info("quotes: starting quote vote!")
    await message.add_reaction("😍")

REACTION_EMOJI = "😍"
REQUIRED_SCORE = 2
async def on_react(payload):
    message = await client.get_channel(payload.channel_id).fetch_message(payload.message_id)
    if message.author != client.user:
        return
    if not "## Quote Message" in message.content:
        return
    if payload.emoji != discord.PartialEmoji.from_str(REACTION_EMOJI):
        await log.info(f"uh oh :( {payload.emoji}")
        return
    
    await log.info("quotes: reaction detected!")
    score = -1 # defaults to -1 to account for the bot's react
    for x in message.reactions:
        if payload.burst:
            await log.info("quotes: super react detected. but they DON'T WORK!! :(")
            message.channel.send("i can't test super reacts yet, so they don't work currently. sorry!!")
        else:
            if x.emoji == REACTION_EMOJI:
                await log.info("counting emoji")
                score += 1 * x.count

    if score >= REQUIRED_SCORE:
        link = await get_quote_message_link(message.content)
        await add_quote(link)

async def get_quote_message_link(message):
    link_match = re.search("(http|https):\/\/(discord(?:(?:\.com+)+))([\w.,@?^=%&:\/~+#-]*[\w@?^=%&\/~+#-])", message)
    if link_match == None:
        await log.info("quotes: no link match found??")
        return None
    await log.info(link_match.group())
    return link_match.group()

QUOTES_CHANNEL_ID = 1272092100795830313

async def add_quote(message_link):
    matches = re.findall("\d+", message_link)
    if matches == None:
        return

    await log.info(matches[1])
    channel = client.get_channel(int(str(matches[1])))
    message = await channel.fetch_message(int(str(matches[2])))

    qchannel = client.get_channel(QUOTES_CHANNEL_ID)
    await qchannel.send(f"\"{message.content}\"\n \- <@{message.author.id}>, <t:{round(message.created_at.timestamp())}:f>")
    await log.info("quotes: quote added!")
