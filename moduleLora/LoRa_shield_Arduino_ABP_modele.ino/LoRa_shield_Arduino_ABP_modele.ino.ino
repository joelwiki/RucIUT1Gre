#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// ######################## Champs à compléter #############################

// LoRaWAN NwkSKey, network session key (MSB first !!!)
static const PROGMEM u1_t NWKSKEY[16] = { 0x86, 0x2E, 0x51, 0x8F, 0x4A, 0x1F, 0xD7, 0x2C, 0x1D, 0xE8, 0x85, 0x1F, 0xA5, 0x4F, 0x09, 0x9E};

// LoRaWAN AppSKey, application session key (MSB first !!!)
static const u1_t PROGMEM APPSKEY[16] = { 0x22, 0xFD, 0x4C, 0x7D, 0x7F, 0x97, 0x40, 0x25, 0xE2, 0xC6, 0xD8, 0x4A, 0x06, 0xEB, 0x78, 0xA2};

// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR =  0x26011EC9;

// #########################################################################

// fonctions utiles uniquement pour OTAA, mais définitions tout de même nécessaires
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

static osjob_t sendjob;
byte i = 2,cpt=2;
char data[] ="Je suis un programme";

// intervalle de transmission en secondes
const unsigned TX_INTERVAL = 10;

// Pin mapping
const lmic_pinmap lmic_pins = 
{
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

void onEvent (ev_t ev) 
{
    Serial.print(os_getTime());
    Serial.print(": ");
    
  
}


void do_send(osjob_t* j)
{
        Serial.println(data);
        LMIC_setTxData2(i, data,sizeof(data), 0);
        Serial.println(F("Packet queued"));
}
    // Next TX is scheduled after TX_COMPLETE event.


void setup() 
{
    Serial.begin(115200);
    Serial.println(F("Starting"));

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);



    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//  LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
//  LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_disableChannel(1);
    LMIC_disableChannel(2);

//  LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//  LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//  LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//  LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//  LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
//  LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);

    // Start job

 
      do_send(&sendjob);
   
}

void loop() 
{
    os_runloop_once();
}
