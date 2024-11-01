# What is Hazelbot?
Hazelbot is my own custom Discord bot, made originally for use in my discord server, but is very customizable. If you're willing to host it yourself, you can use it for any of your own discord servers. It's also open source, so, get this, you can contribute to it yourself. Or fork it and develop your own version of it or whatever. Although, if you're planning on going into the code, just be aware that this is my first time working in C++, so there might be.. horrible things in there..

## Features
### Chat Interactions
Hazelbot is capable of very simple interactions, involving searching messages for a keyword, and then picking a random response to it. The random responses are fully customizable, but the words it responds to aren't, yet.

### Eightball
Hazelbot can detect if a message is a very simple yes or no question, and will respond to it similarly to the chat interactions (but it is assumed the answers you give it are appropriate for a yes/no question). The way this works is, if a message ends with a question mark, it will then check the start of the message to see if it directly addresses Hazelbot, and contains a valid question starting. For example:<br>
"Hazelbot, do you like Pizza?" is a valid question<br>
"hazelbot do you love me?" is a valid question<br>
"hazelbot do you enjoy Murder" is not a valid question, because it doesn't contain a question mark<br>
"do you like oranges?" is not a valid question, as it doesn't directly address Hazelbot<br>
"Hazelbot, what is your opinion on the current state of the economy?" is not a valid question, as it doesn't contain a valid question starting (it is detected to not be a yes/no question)

### 2
Forces every message within a specified channel to contain the number 2, in some form. For example:<br>
"2" is acceptable<br>
"i went two the shops today" is acceptable<br>
"i went to the shops today" is not acceptable, and will be deleted

### Quote
Hazelbot allows you to quote messages that people send, and have them saved to a channel. When you open the context menu on a message in Discord, you will see a expandable menu called "Apps". Inside of which will be a button that says "Quote" with Hazelbot's profile picture. When you press this button, Hazelbot will send a message to that channel telling users that you would like to quote that message, and telling them to react with a specified emoji in order to have it saved to the quotes channel. If the message reaches sufficient reactions, then it will be saved. Otherwise it won't be, and the message will be forgotten by the bot after a while (reacting to the message will no longer send it to the quotes channel)

### Counting
Creates a counting channel, where users can count in increments of one, with the goal of reaching the highest number possible. But whenever someone sends the wrong number, or sends a number twice in a row, the entire chain will be reset.
This comes with an associated `/cstats` command, which can allow you to see all of the stats for the server, including total counts, the highest chain ever reached, and who failed it, etc. You can also add a user to this command, to see stats for that user specifically. There will also be a leaderboard for total counts, total failures, and highest failure, but that will come later.

### Clips
This works similarly to the quote system. When a message containing a valid clip link (currently hardcoded to work for my twitch channel only, but this will be customizable in the future) is posted in the clips channel, you will be able to vote on it. If it receives enough votes, it will be added to the top clips channel. This only detects clips, so any discussion in the clips channel is allowed.

# Configuration
Hazelbot has been designed to be customizable, and allows you to enable and disable each of its modules, as well as change their behaviour, and additionally allows you to add your own responses to the bot.

## `config/hazelbot.cfg`
The main Hazelbot config file uses its own config parser, the syntax to which is as follows: `key=value`. The keys can be written in any order, or just omitted entirely, in which case they will be set to their default values at runtime. You can also just add random nonsense to the file wherever, and it won't mind. (But it has to have an equals in it, otherwise Hazelbot will just crash on startup. The error handling is kind of lacking lmao)

Here is a list of all config entries, in the format {type} `{key}={default_value}`

### String `token=0`
The token for your Discord bot. You can find this on the discord developer portal. This needs a value otherwise the bot will not be able to start up.

### String `guild_id=0`
The ID for the server you want the bot to run in. This determines where the bot will register commands. If left as zero, it will register commands globally, allowing them to be used in any server the bot is in, *however*, syncing this takes a lot of extra time which can be inconvenient. Additionally, most of the bot's commands currently only work in one server.

### String `quotes_channel_id=0`
The ID for the channel you would like quote messages to be saved to.

### Integer `quote_votes_required=1`
The number of votes required on a quote before it will be saved to the quotes channel

### String `quote_reaction_emoji_name=⭐`
The emoji to be used when voting on quotes. If this is a default emoji, set this to the actual emoji character. If it is a custom emoji, set it to the name of the custom emoji and set the `quote_reaction_emoji_id` field too.

### String `quote_reaction_emoji_id=`
If you're using a custom emoji for your quote voting, set this to the ID of that emoji, as well as setting its name.

### String `counting_channel_id=0`
The ID for the counting channel. Leave this as zero to disable the counting system.

### Counting Emojis
These all work the same as the quote emoji, but for different parts of the counting system. There's a lot of them, so I'm merging them all into one section here to make it easier to reference. Add `_name` or `_id` to each of these to get the correct fields, reference `quote_reaction_emoji_name` for more information

`counting_continue_chain_emoji` - ✅
`counting_continue_chain_2_emoji` - 2️⃣
`counting_continue_chain_one-hundred_emoji` - 💯
`counting_continue_chain_new-highest_emoji` - ☑️
`counting_fail_chain_emoji` - ❌

### String `clips_channel_id=0`
The ID for the channel in which clips will be submitted to. Leave this as zero to disable the clips system.

### String `clip_vote_emoji_name=⭐`
The emoji to be used when voting on clips. If this is a default emoji, set this to the actual emoji character. If it is a custom emoji, set it to the name of the custom emoji and set the `clip_vote_emoji_id` field too.

### String `clip_vote_emoji_id=`
If you're using a custom emoji for your clip voting, set this to the ID of that emoji, as well as setting its name.

### String `top_clips_channel_id=0`
The ID for the channel in which top clips will be saved to. This **must** be set if `clips_channel_id` is.

### String `2_channel_id=0`
The ID for the 2 channel. Leave this as zero to disable the 2 channel.

### Boolean `catgirl_mode=true`
If true, all responses will be replaced with hardcoded "meow" values.

## `config/response/`
In this directory, you can add files which allow you to set custom responses for almost all of Hazelbot's responses. Each new response should be placed on a new line. Every file needs to exist, and have at least one entry currently. If you download the release build, it will be packaged with the response files that the official Hazelbot uses, which you might want to use as a starting point. This system will be improved upon in future. Here's an example of the `hazelbot.txt` file.

```
did someone say my name?
hey ;]
hello!!
:3
SHUT THE ###### #### ##### ###
fine day innit?
i think i've fallen in love with you
thy shall suffer my wrath for proclaiming my name!
haiii :3
heyyyy
i hate you.
YOU MAKE EVEN THE DEVIL CRY
i don't wanna talk rn..
i'm about to go s*gma mode
hi.
hello
greetings
waow hi
life has lost all meaning
^w^
sorry i've been feeling a little upset lately and i don't really feel like talking :(
shut up before i make you.
hey
haii how are you doing! ^^
hai!!!
yay! hi!
ok.
idc. shut up.
https://cdn.discordapp.com/attachments/1277125825447202816/1284132299150987274/IMG_9972.jpg?ex=66ff3a26&is=66fde8a6&hm=49af80338fc25d623d54bcf8d5980725c5269995ea65824656130fe88653937c&
ok but did you hear about the rizzler?
g g g g gg gg  g gg g  g g g gggg gg g gg g gg g g g gg gggg g
CRY ABOUT IT
```

Files don't need to have any file extension, and you can set it to whatever you want since the parser just ignores extensions, but it's recommended to just leave them as .txt.

Here are all the response files:

### `cabbit.txt`
Responds to any message containing the word "cabbit".

### `counting_chain_fail_wrongnumber.txt`
Responds when someone fails a chain in the counting channel by sending the wrong number

### `counting_chain_fail_doublesend.txt`
Responds when someone fails a chain in the counting channel by sending a number twice

### `eightball.txt`
Responds whenever hazelbot detects a yes/no question directed at her. Fill this one with various yes or no answers

### `hazelbot.txt`
Responds to any message containing the bot's username.

### `let there be cabbits.txt`
Responds whenever a message matches "Let there be cabbits!". Not case-sensitive.

### `ping.txt`
Responds whenever you ping Hazelbot.

### `silksong.txt`
Responds to any message containing the word "silksong"

### `SQUISH.txt`
Responds to any message containing the phrase "step 3"

### `strikethrough.txt`
Responds to any message containing `~~hazelbot~~` (which renders on discord as strikethrough)

