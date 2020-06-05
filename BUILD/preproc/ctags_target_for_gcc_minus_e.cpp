# 1 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino"
# 2 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino" 2
# 3 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino" 2

# 5 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino" 2
# 6 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino" 2




int counter;
//U8G2_SH1106_128X64_NONAME_F_HW_I2C gfx(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ PB6, /* data=*/ PB7);

U8G2_SH1106_128X64_NONAME_F_HW_I2C gfx((&u8g2_cb_r0), /* reset=*/ 255, /* clock=*/ 22 /* | 22      |        | USART1_TX  | TWI1_SCL  |            |           |*/, /* data=*/ 23 /* | 23      |        | USART1_RX  | TWI1_SDA  |            |           |*/);

void myDisplayFunction();

void setup() {
   /* for(int i = 0; i<10;i++){

        menuSupportedPID = PID_List[i].name;

    }*/
# 21 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino"
    Wire.begin();
    SerialUSB.begin(115200);
    gfx.begin();
    setupMenu();
    renderer.setResetCallback([] {
        counter = 0;
        renderer.takeOverDisplay(myDisplayFunction);
    });
    renderer.setResetIntervalTimeSeconds(5);

}
void loop() {
    taskManager.runLoop();
}
// this function will be called when the menu becomes inactive.
/*void onMenuBeingReset() {

    // for example in here we could take over the display when the menu is inactive.

    renderer.takeOverDisplay(myDisplayFunction);

}

// This will be called frequently by the renderer class

// here we give control back when the button is clicked.

void myDisplayFunction() {

    // draw something..

   gfx.clearBuffer();

   gfx.setFont(u8g2_font_pxplustandynewtv_8f);

   gfx.setCursor(1,10);

   gfx.print("display something when the menu timeout");

   gfx.sendBuffer();

}

*/
# 51 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino"
// see tcMenu list documentation on thecoderscorner.com
int fnNewSubMenuSupportedPIDLISTRtCall(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize) {
   switch(mode) {
    case RENDERFN_INVOKE:
        // TODO - your code to invoke goes here - row is the index of the item
        return true;
    case RENDERFN_NAME:
        // TODO - each row has it's own name - 0xff is the parent item
        ltoaClrBuff(buffer, row, 3, 0, bufferSize);
        return true;
    case RENDERFN_VALUE:
        // TODO - each row can has its own value - 0xff is the parent item
        buffer[0] = 'V'; buffer[1]=0;
        fastltoa(buffer, row, 3, 0, bufferSize);
        return true;
    case RENDERFN_EEPROM_POS: return 0xffff; // lists are generally not saved to EEPROM
    default: return false;
    }
}
//
// this is the function called by the renderer every 1/5 second once the display is
// taken over, we pass this function to takeOverDisplay below.
//
void myDisplayFunction(unsigned int encoderValue, RenderPressMode clicked) {
    // we initialise the display on the first call.
    if(counter == 0) {
        displayMsg("OK button");
    }
    // We are told when the button is pressed in by the boolean parameter.
    // When the button is clicked, we give back to the menu..
    if(clicked) {
        renderer.giveBackDisplay();
        counter = 0;
    }
    else {
        char buffer[5];
       // otherwise update the counter.
       /* lcd.setCursor(0, 2);

        ltoaClrBuff(buffer, ++counter, 4, ' ', sizeof(buffer));

        lcd.print(buffer);

        lcd.setCursor(12, 2);

        ltoaClrBuff(buffer, encoderValue, 4, '0', sizeof(buffer));

        lcd.print(buffer);*/
# 94 "c:\\ARMTC\\code\\tcMenuOBD\\tcMenuOBD.ino"
    }
}
void displayMsg(char *msg){
   gfx.clearBuffer();
   gfx.setFont(u8g2_font_pxplustandynewtv_8f);
   gfx.setCursor(0,12);
   gfx.print(msg);
   gfx.sendBuffer();
}
//
// We have an option on the menu to take over the display, this function is called when that
// option is chosen.
//
void onTakeOverDisplay(int /*id*/) {
    // in order to take over rendering onto the display we just request the display
    // at which point tcMenu will stop rendering until the display is "given back".
    // Don't forget that LiquidCrystalIO uses task manager and things can be happening
    // in the background. Always ensure all operations with the LCD occur on the rendering
    // call back.
    counter = 0;
    renderer.takeOverDisplay(myDisplayFunction);
}
