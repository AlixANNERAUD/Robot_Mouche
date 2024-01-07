#include "sound.hpp"

void SoundClass::play(std::string file_name)
{
    LOG_INFORMATION("Sound", "Playing sound: %s", file_name.c_str());
    system("pkill -9 mpg321");            // Kill any existing sound
    system("amixer -q set PCM,0 unmute"); // Unmute
    file_name = "mpg321 -q assets/" + file_name + " &";
    // std::string command = "mpg321 -q assets/" + file_name + " &";
    system(file_name.c_str());
}