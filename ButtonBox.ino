//Button Box
//George Marshall

#include "Keyboard.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//button actions: chagne what key each button emulates here
#define BUTT_1_KEY KEY_KP_1
#define BUTT_2_KEY KEY_KP_2
#define BUTT_3_KEY KEY_KP_3
#define BUTT_4_KEY KEY_KP_4

//delays in ms to change how fast each component updates
#define DISPLAY_UPDATE_DELAY    200
#define LED_UPDATE_DELAY        5
#define SEND_KEYPRESS_DELAY     100
#define ESTOP_DELAY             200

//pin and address definitions
#define DISPLAY_ADDRESS 0x70
#define E_STOP_PIN 11
#define F_SWIT_PIN 10
#define BUTT_1_PIN 9
#define BUTT_2_PIN A0
#define BUTT_3_PIN A1
#define BUTT_4_PIN A2

#define BEEMOVIE_LENGTH 10313

Adafruit_AlphaNum4 display = Adafruit_AlphaNum4();

String displayText = "MERRY CHRISTMAS SEAN      ";
char displayBuffer[4];
unsigned int displayTextIndex = 0;

long lastDisplayRefresh = 0;
long lastLedUpdate = 0;
long lastKeypress = 0;

uint8_t ledBrightness = 0;
uint8_t fadeAmount = 1;

int flightSwitchState = 0;
int flightSwitchPreviousState = 0;

int button1State = 0;
int button1PreviousState = 0;
int button2State = 0;
int button2PreviousState = 0;
int button3State = 0;
int button3PreviousState = 0;
int button4State = 0;
int button4PreviousState = 0;

//large string stored in progmem
unsigned int beeMovieIndex = 0;
static const char beeMovieScript[] PROGMEM = "According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway because bees don't care what humans think is impossible. Yellow, black. Yellow, black. Yellow, black. Yellow, black. Ooh, black and yellow! Let's shake it up a little. Barry! Breakfast is ready! Coming! Hang on a second. Hello? Barry? Adam? Can you believe this is happening? I can't. I'll pick you up. Looking sharp. Use the stairs, Your father paid good money for those. Sorry. I'm excited. Here's the graduate. We're very proud of you, son. A perfect report card, all B's. Very proud. Ma! I got a thing going here. You got lint on your fuzz. Ow! That's me! Wave to us! We'll be in row 118,000. Bye! Barry, I told you, stop flying in the house! Hey, Adam. Hey, Barry. Is that fuzz gel? A little. Special day, graduation. Never thought I'd make it. Three days grade school, three days high school. Those were awkward. Three days college. I'm glad I took a day and hitchhiked around The Hive. You did come back different. Hi, Barry. Artie, growing a mustache? Looks good. Hear about Frankie? Yeah. You going to the funeral? No, I'm not going. Everybody knows, sting someone, you die. Don't waste it on a squirrel. Such a hothead. I guess he could have just gotten out of the way. I love this incorporating an amusement park into our day. That's why we don't need vacations. Boy, quite a bit of pomp under the circumstances. Well, Adam, today we are men. We are! Bee-men. Amen! Hallelujah! Students, faculty, distinguished bees, please welcome Dean Buzzwell. Welcome, New Hive City graduating class of 9:15. That concludes our ceremonies And begins your career at Honex Industries! Will we pick our job today? I heard it's just orientation. Heads up! Here we go. Keep your hands and antennas inside the tram at all times. Wonder what it'll be like? A little scary. Welcome to Honex, a division of Honesco and a part of the Hexagon Group. This is it! Wow. Wow. We know that you, as a bee, have worked your whole life to get to the point where you can work for your whole life. Honey begins when our valiant Pollen Jocks bring the nectar to The Hive. Our top-secret formula is automatically color-corrected, scent-adjusted and bubble-contoured into this soothing sweet syrup with its distinctive golden glow you know as... Honey! That girl was hot. She's my cousin! She is? Yes, we're all cousins. Right. You're right. At Honex, we constantly strive to improve every aspect of bee existence. These bees are stress-testing a new helmet technology. What do you think he makes? Not enough. Here we have our latest advancement, the Krelman. What does that do? Catches that little strand of honey that hangs after you pour it. Saves us millions. Can anyone work on the Krelman? Of course. Most bee jobs are small ones. But bees know that every small job, if it's done well, means a lot. But choose carefully because you'll stay in the job you pick for the rest of your life. The same job the rest of your life? I didn't know that. What's the difference? You'll be happy to know that bees, as a species, haven't had one day off in 27 million years. So you'll just work us to death? We'll sure try. Wow! That blew my mind!  What's the difference?  How can you say that? One job forever? That's an insane choice to have to make. I'm relieved. Now we only have to make one decision in life. But, Adam, how could they never have told us that? Why would you question anything? We're bees. We're the most perfectly functioning society on Earth. You ever think maybe things work a little too well here? Like what? Give me one example. I don't know. But you know what I'm talking about. Please clear the gate. Royal Nectar Force on approach. Wait a second. Check it out. Hey, those are Pollen Jocks! Wow. I've never seen them this close. They know what it's like outside The Hive. Yeah, but some don't come back. Hey, Jocks! Hi, Jocks! You guys did great! You're monsters! You're sky freaks! I love it! I love it! I wonder where they were. I don't know. Their day's not planned. Outside The Hive, flying who knows where, doing who knows what. You can't just decide to be a Pollen Jock. You have to be bred for that. Right. Look. That's more pollen than you and I will see in a lifetime. It's just a status symbol. Bees make too much of it. Perhaps. Unless you're wearing it and the ladies see you wearing it. Those ladies? Aren't they our cousins too? Distant. Distant. Look at these two. Couple of Hive Harrys. Let's have fun with them. It must be dangerous being a Pollen Jock. Yeah. Once a bear pinned me against a mushroom! He had a paw on my throat, and with the other, he was slapping me! Oh, my! I never thought I'd knock him out. What were you doing during this? Trying to alert the authorities. I can autograph that. A little gusty out there today, wasn't it, comrades? Yeah. Gusty. We're hitting a sunflower patch six miles from here tomorrow. Six miles, huh? Barry! A puddle jump for us, but maybe you're not up for it. Maybe I am. You are not! We're going 0900 at J-Gate. What do you think, buzzy-boy? Are you bee enough? I might be. It all depends on what 0900 means. Hey, Honex! Dad, you surprised me. You decide what you're interested in? Well, there's a lot of choices. But you only get one. Do you ever get bored doing the same job every day? Son, let me tell you about stirring. You grab that stick, and you just move it around, and you stir it around. You get yourself into a rhythm. It's a beautiful thing. You know, Dad, the more I think about it, maybe the honey field just isn't right for me. You were thinking of what, making balloon animals? That's a bad job for a guy with a stinger. Janet, your son's not sure he wants to go into honey! Barry, you are so funny sometimes. I'm not trying to be funny. You're not funny! You're going into honey. Our son, the stirrer! You're gonna be a stirrer? No one's listening to me! Wait till you see the sticks I have. I could say anything right now. I'm gonna get an ant tattoo! Let's open some honey and celebrate! Maybe I'll pierce my thorax. Shave my antennae. Shack up with a grasshopper. Get a gold tooth and call everybody  dawg ! I'm so proud. We're starting work today! Today's the day. Come on! All the good jobs will be gone. Yeah, right. Pollen counting, stunt bee, pouring, stirrer, front desk, hair removal... Is it still available? Hang on. Two left! One of them's yours! Congratulations! Step to the side. What'd you get? Picking crud out. Stellar! Wow! Couple of newbies? Yes, sir! Our first day! We are ready! Make your choice. You want to go first? No, you go. Oh, my. What's available? Restroom attendant's open, not for the reason you think. Any chance of getting the Krelman? Sure, you're on. I'm sorry, the Krelman just closed out. Wax monkey's always open. The Krelman opened up again. What happened? A bee died. Makes an opening. See? He's dead. Another dead one. Deady. Deadified. Two more dead. Dead from the neck up. Dead from the neck down. That's life! Oh, this is so hard! Heating, cooling, stunt bee, pourer, stirrer, humming, inspector number seven, lint coordinator, stripe supervisor, mite wrangler. Barry, what do you think I should... Barry? Barry! All right, we've got the sunflower patch in quadrant nine... What happened to you? Where are you? I'm going out. Out? Out where? Out there. Oh, no! I have to, before I go to work for the rest of my life. You're gonna die! You're crazy! Hello? Another call coming in. If anyone's feeling brave, there's a Korean deli on 83rd that gets their roses today. Hey, guys. Look at that. Isn't that the kid we saw yesterday? Hold it, son, flight deck's restricted. It's OK, Lou. We're gonna take him up. Really? Feeling lucky, are you? Sign here, here. Just initial that. Thank you. OK. You got a rain advisory today, and as you all know, bees cannot fly in rain. So be careful. As always, watch your brooms, hockey sticks, dogs, birds, bears and bats. Also, I got a couple of reports of root beer being poured on us. Murphy's in a home because of it, babbling like a cicada! That's awful. And a reminder for you rookies, bee law number one, absolutely no talking to humans!  All right, launch positions! Buzz, buzz, buzz, buzz! Buzz, buzz, buzz, buzz! Buzz, buzz, buzz, buzz! Black and yellow! Hello! You ready for this, hot shot? Yeah. Yeah, bring it on. Wind, check. Antennae, check. Nectar pack, check. Wings, check. Stinger, check. Scared out of my shorts, check. OK, ladies, let's move it out! Pound those petunias, you striped stem-suckers! All of you, drain those flowers! Wow! I'm out! I can't believe I'm out! So blue. I feel so fast and free! Box kite! Wow! Flowers! This is Blue Leader, We have roses visual. Bring it around 30 degrees and hold. Roses! 30 degrees, roger. Bringing it around. Stand to the side, kid. It's got a bit of a kick. That is one nectar collector! Ever see pollination up close? No, sir. I pick up some pollen here, sprinkle it over here. Maybe a dash over there, a pinch on that one. See that? It's a little bit of magic. That's amazing. Why do we do that? That's pollen power. More pollen, more flowers, more nectar, more honey for us. Cool. I'm picking up a lot of bright yellow, Could be daisies, Don't we need those? Copy that visual. Wait. One of these flowers seems to be on the move. Say again? You're reporting a moving flower? Affirmative. That was on the line! This is the coolest. What is it? I don't know, but I'm loving this color. It smells good. Not like a flower, but I like it. Yeah, fuzzy. Chemical-y. Careful, guys. It's a little grabby. My sweet lord of bees! Candy-brain, get off there! Problem! Guys! This could be bad. Affirmative. Very close. Gonna hurt. Mama's little boy. You are way out of position, rookie! Coming in at you like a missile! Help me! I don't think these are flowers. Should we tell him? I think he knows. What is this?! Match point! You can start packing up, honey, because you're about to eat it! Yowser! Gross. There's a bee in the car! Do something! I'm driving! Hi, bee. He's back here! He's going to sting me! Nobody move. If you don't move, he won't sting you. Freeze! He blinked! Spray him, Granny! What are you doing?!";


void setup()
{
    Serial.begin(9600);

    display.begin(DISPLAY_ADDRESS);
    display.setBrightness(4);
    display.clear();
    display.writeDisplay();

    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(E_STOP_PIN, INPUT_PULLUP);
    pinMode(F_SWIT_PIN, INPUT);
    pinMode(BUTT_1_PIN, INPUT_PULLUP);
    pinMode(BUTT_2_PIN, INPUT_PULLUP);
    pinMode(BUTT_3_PIN, INPUT_PULLUP);
    pinMode(BUTT_4_PIN, INPUT_PULLUP);

    delay(10);

    Keyboard.begin();
}

void loop()
{
    UpdateButtons();
    PulseLed();
    UpdateDisplay();
}

//checks each button if it is pressed and sends emulated keypresses to the PC via USB
void UpdateButtons()
{
    long currentMillis = millis();

    if(currentMillis - lastKeypress > SEND_KEYPRESS_DELAY)
    {
        flightSwitchState = digitalRead(F_SWIT_PIN);
        button1State = !digitalRead(BUTT_1_PIN);
        button2State = !digitalRead(BUTT_2_PIN);
        button3State = !digitalRead(BUTT_3_PIN);
        button4State = !digitalRead(BUTT_4_PIN);

        if(!digitalRead(E_STOP_PIN))
        {
            display.writeDigitAscii(0, 'S');
            display.writeDigitAscii(1, 'T');
            display.writeDigitAscii(2, 'O');
            display.writeDigitAscii(3, 'P');
            display.writeDisplay();

            Keyboard.press(KEY_LEFT_ALT);
 
            while(!digitalRead(E_STOP_PIN))
            {
                Keyboard.write(KEY_F4);
                delay(ESTOP_DELAY);
            }

            Keyboard.release(KEY_LEFT_ALT);

            display.clear();
            display.writeDisplay();
        }
    
        if(flightSwitchState != flightSwitchPreviousState || beeMovieIndex >= BEEMOVIE_LENGTH)
        {
            beeMovieIndex = 0;
        }  
        
        if(flightSwitchState)
        {   
            Keyboard.write((char) pgm_read_byte(&beeMovieScript[beeMovieIndex++]));
        }

        if(button1State != button1PreviousState && button1State == LOW)
        {
            Keyboard.write(BUTT_1_KEY);
        }

        if(button2State != button2PreviousState && button2State == LOW)
        {
            Keyboard.write(BUTT_2_KEY);
        }

        if(button3State != button3PreviousState && button3State == LOW)
        {
            Keyboard.write(BUTT_3_KEY);
        }

        if(button4State != button4PreviousState && button4State == LOW)
        {
            Keyboard.write(BUTT_4_KEY);
        }
  
        flightSwitchPreviousState = flightSwitchState;
        button1PreviousState = button1State;
        button2PreviousState = button2State;
        button3PreviousState = button3State;
        button4PreviousState = button4State;

        lastKeypress = currentMillis;
    }
}

//scrolls the display text string across the display
void UpdateDisplay()
{
    long currentMillis = millis();

    if(currentMillis - lastDisplayRefresh > DISPLAY_UPDATE_DELAY)
    {
        if(displayTextIndex >= displayText.length())
            displayTextIndex = 0;

        for(int i = 0; i < 3; i++)
            displayBuffer[i] = displayBuffer[i + 1];

        displayBuffer[3] = displayText.charAt(displayTextIndex);
        displayTextIndex++;
    
        for(int i = 0; i < 4; i++)
            display.writeDigitAscii(i, displayBuffer[i]);
        
        display.writeDisplay();

        lastDisplayRefresh = currentMillis;
    }
}

//slowly fades the status led
void PulseLed()
{
    long currentMillis = millis();

    if(currentMillis - lastLedUpdate > LED_UPDATE_DELAY)
    {
        ledBrightness += fadeAmount;
        analogWrite(LED_BUILTIN, ledBrightness);
        lastLedUpdate = currentMillis;

        if(ledBrightness == 0 || ledBrightness == 255)
            fadeAmount = -fadeAmount;
    }
}




