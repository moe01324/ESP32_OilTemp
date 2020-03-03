/**
 *	VDO Temperature Sender
 */
#ifndef VdoTemperatureSender_h
#define VdoTemperatureSender_h

#include <Arduino.h>

class VdoTemperatureSender
{
	public: 
		VdoTemperatureSender(int analogPin, unsigned int dataTable[16]);
		int getTemperature();
		void debug(bool debug);

	private:
		int _analogPin;
		bool _debug;
		int _checkOrderTable;
		unsigned int _dataTable[16];
		int checkOrder();
};

#endif