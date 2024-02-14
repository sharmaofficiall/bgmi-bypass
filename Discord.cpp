#include "Discord.h"
#include "chrono"

static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void Discord::Initialize()
{
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));
    Discord_Initialize("1155556410386567311", &Handle, 1, NULL);
}


//back up
void Discord::Update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Playing BGMI 3.0";
  //  discordPresence.details = "SHARMA BYPASS BGMI | https://discord.gg/VKUnnZ9h7B";
    std::time_t CurrentTime = std::time(0); discordPresence.startTimestamp = CurrentTime;   // discordPresence.endTimestamp = 1507665886;
    discordPresence.largeImageKey = "https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExMWc3M202MHU3NndhM3lma2psdng2OGdkbGY4aGtuZWprbGZyaXh0eiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/7t6n7VF3VJsWoqXjxX/giphy.gif";
   // discordPresence.smallImageText = "Rogue - Level 100";
   // discordPresence.largeImageText = "SHARMA BYPASS BGMI";
    discordPresence.smallImageKey = "https://c.tenor.com/TgKK6YKNkm0AAAAi/verified-verificado.gif";
    discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
    discordPresence.partySize = 1;
    discordPresence.partyMax = 4;
    discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
    Discord_UpdatePresence(&discordPresence); 


  //  DiscordRichPresence discordPresence;
  //  memset(&discordPresence, 0, sizeof(discordPresence));
  //  discordPresence.state = "Playing BGMI 2.7 | Gameloop";
  //  discordPresence.details = "SHARMA PRIVATE BYPASS BGMI | https://discord.gg/VKUnnZ9h7B";
   // discordPresence.largeImageKey = "largeimage";
    //discordPresence.largeImageText = "SHARMA PRIVATE BYPASS";
    //discordPresence.smallImageText = "Rogue - Level 100";
    //discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
    //discordPresence.partySize = 1;
    //discordPresence.partyMax = 5;
    //discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
   // Discord_UpdatePresence(&discordPresence); 


}


