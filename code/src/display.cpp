#include "main.hpp"

#include "pin.hpp"
#include "lcd.hpp"
#include "log.hpp"

#include <thread>
#include <string>
#include <cstring>

void display(PinClass &SDA, PinClass &SCL)
{
    std::thread([&SDA, &SCL]()
                {
                LCDClass lcd(SDA, SCL);
 
                if (!lcd.isValid())
                {
                    LOG_ERROR("LCD", "Failed to initialize LCD.");
                    return;
                }

                lcd.setCursor(0, 0);
                lcd.printFormatted("- Robot Mouche -");
                
 
 
                const char *lyrics = "Never gonna give you up !\n"
                                     "Never gonna let you down !\n"
                                     "Never gonna run around and desert you !\n"
                                     "Never gonna make you cry !\n"
                                     "Never gonna say goodbye !\n"
                                     "Never gonna tell a lie and hurt you !\n";
                unsigned int i = 0;
                std::string currentLine;
                currentLine.reserve(16);
                while (true)
                {
                    currentLine = "";
        
                    while (lyrics[i] != '\n' && (i < strlen(lyrics)))
                    {
                        currentLine += lyrics[i];
                        i++;

                    }
                    i++;
                    
                    if (i >= strlen(lyrics))
                        i = 0;

                    LOG_VERBOSE("LCD", "%s", currentLine.c_str());

                

                    for (size_t j = 16; j > 0; j-=2)
                    {
                        lcd.setCursor(j, 1);
                        lcd.printFormatted(currentLine.substr(0, 16 - j).c_str());
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    }

                    for (size_t j = 0; j < currentLine.size(); j+=2)
                    {
                        lcd.setCursor(0, 1);
                        lcd.printFormatted(currentLine.substr(j, 16).c_str());
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    }

                    // Outro

                    for (size_t j = 16; j > 0; j-=2)
                    {
                        lcd.setCursor(j, 1);
                        lcd.printFormatted(currentLine.substr(0, 16 - j).c_str());
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    }

                } })
        .detach();
}