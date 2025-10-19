/* Including the SDK and WiFi library */
#include <Grandeur.h>
#include <ESP8266WiFi.h>


/* Configurations */
String apiKey = "grandeurlgt4svaggqsu0jlug9uq6sji";
String deviceID = "devicelgt4sw1ugqsy0jlubt5269qt";
String token = "83bc2a6e08c94bb3e6a560aeac93c1c44f4cdbbbff779037b8575c9c17536330";


/* WiFi credentials */
const char* ssid = YOUR_SSID;
const char* password = YOUR_PASSWORD;

/* Create variable to hold project and device */
Grandeur::Project project;
Grandeur::Project::Device device;

/* Function prototypes */
void onConnection(bool status);
void updateHandler(const char* path, const char* state);
void connectWiFi();

void setup() {
    /* Begin the serial */
    Serial.begin(9600);

    /* Connect to WiFi */
    connectWiFi();

    /* Initializes the global object "grandeur" with your configurations. */
    project = grandeur.init(apiKey, token);

    /* Get reference to device */
    device = project.device(deviceID);

    /* Sets connection state update handler */
    project.onConnection(onConnection);

    /* Add event handler on state variable */
    device.data().on("state", updateHandler);

    /* Set mode of LED to output */
    pinMode(LED_BUILTIN, OUTPUT);

    /* Turn the LED off by default */
    digitalWrite(LED_BUILTIN, 0);
}

void loop() {
    /*  Runs the SDK.
        Connects to Grandeur only after the device is connected to WiFi.
    */
    if(WiFi.status() == WL_CONNECTED) project.loop();
}

/* Function to check device's connection status */
void onConnection(bool status) {
    switch(status) {
        case CONNECTED:
            /* Device connected to the cloud */
            
            Serial.println("Device is connected to the cloud.");

            /* Getting the previous state */
            device.data().get("state", updateHandler);
            return;

        case DISCONNECTED:
            /* Device disconnected from cloud */

            Serial.println("Device is disconnected from the cloud.");
            return;
    }
}

/* Function to handle state update event */
void updateHandler(const char* path, const char* state) {
    /* Converting from string to int */
    int stateValue = atoi(state);

    /* Print state */
    Serial.printf("Updated state is %d
", stateValue);

    /* Update pin level */
    digitalWrite(LED_BUILTIN, !stateValue);
}

/* Function to connect to WiFi */
void connectWiFi() {
    /* Set mode to station */
    WiFi.mode(WIFI_STA);

    /* Connect using the ssid and password */
    WiFi.begin(ssid, password);

    /* Block till the WiFi is connected */
    while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
    }

    /* Print message */
    Serial.println("");
    Serial.println("WiFi connected");

    /* And IP address */
    Serial.println(WiFi.localIP());
}

        
