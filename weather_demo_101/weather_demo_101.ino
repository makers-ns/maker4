/*******************************************************************************
 * Title                 :   Remote Weather Station
 * Filename              :   RemoteWeather.ino
 * Author                :   RL
 * Origin Date           :   26/11/2015
 * Notes                 :   Running on Flip and Click Due CPU
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
 *
 *    Date    Software Version    Initials   Description
 *  25/10/2015    .1               RL        Module Created
 *  26/10/2015    .2               RL        Added timers
 *  26/10/2015    .3               RL        Added interrupts and timer
 *
 *******************************************************************************/
/**
 *  @file RemoteWeather.ino
 *  @brief Demonstrates runing multiple click boards on flip and click board
 *  and transmission of the various sensor readings to a coordinator.
 *  
 *  This example uses the following boards:
 *  Main Board - Flip and Click
 *  
 *  <b>Click Boards:</b>
 *  <ul>
 *  <li>XBee click</li>
 *  <li></li>
 *  <li>Weather Click</li>
 *  <li>Ambient 2 Click</li>
 *  </ul>
 *  
 *  The following libraries are used:
 *  <ul>
 *  <li>Wire</li>
 *  <li>SPI</li>
 *  <li>DueTimer</li>
 *  <li>flip_click_defs</li>
 *  </ul>
 */
/******************************************************************************
 * Includes
 *******************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/
// Ambient

#define SEALEVELPRESSURE_HPA (1013.25)

/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/
#define get_temp()               weather_data.temp
#define get_humidity()           weather_data.humidity
#define get_pressure()           weather_data.pressure
#define get_ambient()            weather_data.ambient_light


/******************************************************************************
 * Module Typedefs
 *******************************************************************************/
typedef struct
{
    float temp;
    float humidity;
    float pressure;
    float ambient_light;
} weather_data_t;

enum
{
    TEMPERATURE = 0,
    PRESSURE,
    HUMIDITY,
    AMBIENT,
    LIGHTNING
};

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/
static weather_data_t weather_data;

// When to update and transmit
volatile bool update_flag;

Adafruit_BME280 bme; 
/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
void bme_init( void );
void bme_update( void );

void weather_update( void );
void transmit( void );


/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void setup() 
{
  SPI.begin();
  Wire.begin();
  Serial.begin( 57600 );  // For debugging
  while(!Serial);
  Serial.println("Serial initialized");
  bme_init();
  Serial.println("End of setup");
  
}

void loop() 
{

      weather_update();
      transmit();
  Serial.println("Loop");
  delay(1000);
}

void bme_init()
{
  if( !bme.begin( 0x76 ) ) 
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void bme_update()
{
    weather_data.temp = bme.readTemperature();
    weather_data.pressure = bme.readPressure() / 100.0f;
    weather_data.humidity = bme.readHumidity();
}

void weather_update()
{
   bme_update();  
}

void transmit()
{
  char txt[50];
  sprintf( txt, "T:%2.1f,P:%3.0f,H:%2.0f", get_temp(), get_pressure(), get_humidity() );
  Serial.println( txt );
}

