// This #include statement was automatically added by the Particle IDE.
#include <ThingSpeak.h>

// This #include statement was automatically added by the Particle IDE.
#include <HC_SR04.h>


#define VCC D7 // I like to use D7 as VCC because the board has a D7 LED indicator. This way we know for sure that it is high.
#define ECHO D12 // defining the Echo Pin
#define TRIGGER D11 // defining the Trigger Pin
double cm = 0.0; // will be used for our readings
double inches = 0.0;
double start = 0.0; // used to measure start  time
double end = 0.0; // used to measure end time
double ttp = 0.0; // ttp = Time To Publish


HC_SR04 motion_detector = HC_SR04(TRIGGER, ECHO); // creating our HC_SR04 object



// Setting Up Connection to ThingSpeak
TCPClient client;

unsigned int channel_ID = 668598; // this is my ThingSpeak Channel ID
const char * write_API_Key = "3VUKIZU2YT62DIX1"; // This is my ThingSpeak API Key


void setup() {

ThingSpeak.begin(client); // starting ThingSpeak
pinMode(VCC, OUTPUT); // Setting VCC Pin as an output
digitalWrite(VCC, HIGH); //Setting VCC to 5V


}

void loop() {
    
    delay(1000);  //let sensors settle
    
   if (motion_detector.getDistanceCM()> 0.0){ // if motion is detected (reading is greater than 0)
        
        Particle.publish("Motion Detected!","HC_SR04",60,PRIVATE); // publish to the Particle Cloud 
        
        cm = motion_detector.getDistanceCM(); // retrieve distance in centimeters
        inches = motion_detector.getDistanceInch(); // retreive distance in inches
        
        ThingSpeak.setField(1, (float)cm); // setting fields for ThingSpeak
        ThingSpeak.setField(2, (float)inches);
        
        //start = millis(); // get time before particle cloud publish
        Particle.publish("Readings", String::format("{\"Distance in (cm)\": %4.2f, \"Distance in (in)\": %4.2f", cm, inches)); // this will post our results to the Particle Cloud
        
        ThingSpeak.writeFields(channel_ID, write_API_Key); // Piublishing to ThingSpeak
        Mesh.publish("Readings", String::format("{\"Distance in (cm)\": %4.2f, \"Distance in (in)\": %4.2f", cm, inches)); // this will public to our mesh network
        
        //end = millis(); // get time after publish delay
        //ttp =  end - start; // determines how much time it takes to publish
        
        Particle.publish("Execution time", String::format("{\"Execution time (s)\": %4.2f, \"Start Time \": %4.2f, \"End Time \": %4.2f" ,ttp,start, end));
        //System.sleep(30); // this will put the device in to sleep mode for 30 seconds.
        

        
        
    }
    
    else{
        Particle.publish("Waiting for motion...");
    }
    
}


