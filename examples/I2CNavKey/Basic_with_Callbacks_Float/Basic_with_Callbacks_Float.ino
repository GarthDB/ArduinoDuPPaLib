#include <Wire.h>
#include <I2CNavKey.h>

/*This is a basic example for using the I2C navkey V2
  Every time a button is pressed it's printed on the terminal.
  The double push is active, with a 300ms window
  The encoder is set from 0.5 to 9.5 with step of 0.25


  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/


const int IntPin = A3; /* Definition of the interrupt pin*/
//Class initialization with the I2C addresses
i2cNavKey navkey(0b0010000); /* Default address when no jumper are soldered */


uint8_t pwm, fade = 0;

void UP_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button UP Pressed!");

}

void DOWN_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button DOWN Pressed!");

}

void LEFT_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button LEFT Pressed!");

}

void RIGHT_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button RIGHT Pressed!");

}

void CENTRAL_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button Central Pressed!");
}

void CENTRAL_Button_Double(i2cNavKey* p) {
  Serial.println("Button Central Double push!");
}

void Encoder_Rotate(i2cNavKey* p) {
  Serial.println(p->readCounterFloat(),2);

}
void setup(void)
{
  pinMode(IntPin, INPUT);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("**** I2C navkey V2 basic example ****");
  /*
      INT_DATA= The register are considered integer.
      WRAP_ENABLE= The WRAP option is enabled
      DIRE_RIGHT= navkey right direction increase the value
      IPUP_ENABLE= INT pin have the pull-up enabled.
  */

  navkey.reset();
  navkey.begin(i2cNavKey::FLOAT_DATA | i2cNavKey::WRAP_ENABLE | i2cNavKey::DIRE_RIGHT | i2cNavKey::IPUP_ENABLE);

  navkey.writeCounter((float)0); /* Reset the counter value */
  navkey.writeMax((float)9.5); /* Set the maximum threshold*/
  navkey.writeMin((float)0.5); /* Set the minimum threshold */
  navkey.writeStep((float)0.25); /* Set the step to 1*/

  navkey.writeDoublePushPeriod(30);  /*Set a period for the double push of 300ms */

  navkey.onUpPush = UP_Button_Pressed;
  navkey.onDownPush = DOWN_Button_Pressed;
  navkey.onRightPush = RIGHT_Button_Pressed;
  navkey.onLeftPush = LEFT_Button_Pressed;
  navkey.onCentralPush = CENTRAL_Button_Pressed;
  navkey.onCentralDoublePush = CENTRAL_Button_Double;
  navkey.onChange = Encoder_Rotate;

  navkey.autoconfigInterrupt(); /* Enable the interrupt with the attached callback */

  Serial.print("ID CODE: 0x");
  Serial.println(navkey.readIDCode(), HEX);
  Serial.print("Board Version: 0x");
  Serial.println(navkey.readVersion(), HEX);

}

void loop() {
  uint8_t enc_cnt;
  if (digitalRead(IntPin) == LOW) {
    navkey.updateStatus();
  }
}
