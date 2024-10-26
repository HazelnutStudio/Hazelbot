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

### String `clips_channel_id=0`
The ID for the channel in which clips will be submitted to. Leave this as zero to disable the clips system.

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
Responds to any message containing the word "hazelbot".

### `let there be cabbits.txt`
Responds whenever a message matches "Let there be cabbits!". Not case-sensitive.

### `ping.txt`
Responds whenever you ping Hazelbot. Currently this is hardcoded to only respond when you ping the official Hazelbot.

### `silksong.txt`
Responds to any message containing the word "silksong"

### `SQUISH.txt`
Responds to any message containing the phrase "step 3"

### `strikethrough.txt`
Responds to any message containing `~~hazelbot~~` (which renders on discord as strikethrough)

