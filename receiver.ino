/*
 * This is a sample project that simple deals with receiving info from a sensor. It's a very
 * simple implementation of the RF24Network library.
 *
 * When it receives the data it converts the data to JSON and prints it to the console.
 *
 * This is an edited version of something that actually works, but I haven't tested this particular
 * varsion works. *shrug*
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include <string.h>
#include <stdlib.h>

// Library to convert data into JSOn
#include <ArduinoJson.h>

// nRF24L01(+) radio attached to ports 9 and 10
RF24 radio(9,10);

// Create the network for the attached radio
RF24Network network(radio);

// Address of this receiver node
const uint16_t this_node = 0;

// The channel that our radio will communicate over
const uint16_t channel = 90;

// The payload that we're expecting to receive
struct payload_t {
	float value;
	unsigned long nodeId;
};

void setup(void)
{
	Serial.begin(9600);

	SPI.begin();
	radio.begin();
	network.begin(channel, this_node);
}

void loop(void)
{
	// Pump the network regularly
	network.update();

	// Is there anything ready for us?
	while ( network.available() )
	{
		// If so, grab it and print it out
		RF24NetworkHeader header;
		network.peek(header);

		// TODO: something with the header here, probably

		// Create the payload for the data
		payload_t payload;

		// Read the data
		network.read(header, &payload, sizeof(payload));

		// Convert data to JSON
		StaticJsonBuffer<200> jsonBuffer;

		JsonObject& root = jsonBuffer.createObject();
		root["value"] = payload.value;
		root["nodeId"] = payload.nodeId;

		// Print out the received data with the new line character as the delimiter
		root.printTo(Serial);
		Serial.print('\n');
	}
}
