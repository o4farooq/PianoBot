const int COMBOS = 64;
const int MAX_NOTES = 100;

void configureAllSensors() // TO BE MODIFIED.
{
	SensorType[S1] = sensorEV3_Touch;
	SensorType[S2] = sensorEV3_Ultrasonic;
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

void storeValue(int tempStoredValues[MAX_NOTES], int colour1[COMBOS], int colour2[COMBOS], int noteIndex)
{
	bool noteAchieved = false;
	for(int noteValue = 0; noteValue < COMBOS && noteAchieved == false; noteValue++)
	{
		if(SensorValue[S1] == colour1[noteValue] && SensorValue[S2] == colour2[noteValue])
		{
			tempStoredValues[noteIndex] = noteValue;
			noteAchieved = true;
		}
	}
}

void readAndStore(int tempStoredValues[MAX_NOTES], int colour1[COMBOS], int colour2[COMBOS])
{
	// Clear array
	for(int i = 0; i < MAX_NOTES; i++)
		tempStoredValues[i] = 0;

	int col_1 = 0, col_2 = 0;
	int noteIndex = 0;

	// While reading blank paper, do nothing.
	while(SensorValue[S1] == (int)colorWhite && SensorValue[S2] == (int)colorWhite)
	{}

	// While not reading white-white, continue reading and storing.
	while(SensorValue[S1] != (int)colorWhite && SensorValue[S2] != (int)colorWhite)
	{
		col_1 = SensorValue[S1];
		col_2 = SensorValue[S2];

		storeValue(tempStoredValues, colour1, colour2, noteIndex);
		noteIndex++;

		// While robot hasn't reached next set of colours, do nothing. Assume we don't have two same notes right after the other.
		while(SensorValue[S1] == col_1 && SensorValue[S2] == col_2)
		{}
	}
}

void playFromFile(ifstream & fin, int angle[COMBOS], int time[COMBOS]) // call this function in the save or play function
{
	int noteValue = 0; // note values stored in the txt file
	int angleOriginal = 0;
	int angleNew = 0;

	while (fin >> noteValue) // Assuming that nMotorEncoder = 0 at the leftmost position of the finger
	{
		angleNew = angle[noteValue];
		if(angleNew > angleOriginal) // go clockwise
		{
			motor[motorA] = 50; // change motorA to whatever motor we use to turn finger
			while(nMotorEncoder[motorA] != angleNew)
			{}
			motor[motorA] = 0;
			clearTimer[T1];
			motor[motorB] = 100;
			wait1Msec(time[noteValue]) // wait for the time stored in the note time array (might have to convert to milliseconds)
			motor[motorB] = -100;
			while(nMotorEncoder[motorA] > 0)
			{}
			motor[motorB] = 0;
		}
		else // go counterclockwise
		{
			motor[motorA] = -50; // change motorA to whatever motor we use to turn finger
			while(nMotorEncoder[motorA] != angleNew)
			{}
			motor[motorA] = 0;
			clearTimer[T1];
			motor[motorB] = 100;
			wait1Msec(time[noteValue]) // wait for the time stored in the note time array (might have to convert to milliseconds)
			motor[motorB] = -100;
			while(nMotorEncoder[motorA] > 0)
			{}
			motor[motorB] = 0;
		}
		angleOriginal = angleNew;
	}
}

void playFromReader(tempStoredValues[MAX_NOTES], int angle[COMBOS], int time[COMBOS])
{
	int noteValue = 0;
	int angleOriginal = 0;
	int angleNew = 0;

	for(int i = 0; i < MAX_NOTES && tempStoredValues[i] != 0; i++) // tempStoredValues[i] == 0 means end of song.
	{
		noteValue = tempStoredValues[i];
		angleNew = angle[noteValue];
		if(angleNew > angleOriginal) // go clockwise
		{
			motor[motorA] = 50; // change motorA to whatever motor we use to turn finger
			while(nMotorEncoder[motorA] != angleNew)
			{}
			motor[motorA] = 0;
			clearTimer[T1];
			motor[motorB] = 100;
			wait1Msec(time[noteValue]) // wait for the time stored in the note time array (might have to convert to milliseconds)
			motor[motorB] = -100;
			while(nMotorEncoder[motorA] > 0)
			{}
			motor[motorB] = 0;
		}
		else // go counterclockwise
		{
			motor[motorA] = -50; // change motorA to whatever motor we use to turn finger
			while(nMotorEncoder[motorA] != angleNew)
			{}
			motor[motorA] = 0;
			clearTimer[T1];
			motor[motorB] = 100;
			wait1Msec(time[noteValue]) // wait for the time stored in the note time array (might have to convert to milliseconds)
			motor[motorB] = -100;
			while(nMotorEncoder[motorA] > 0)
			{}
			motor[motorB] = 0;
		}
		angleOriginal = angleNew;
	}
}

void buttonOptions(ifstream & saveUp, ifstream & saveLeft, ifstream & saveDown, ifstream & saveRight, int angle[COMBOS], int time[COMBOS])
{
	if(getButtonPress(buttonUp))
	{
		while(getButtonPress(buttonUp))
		{}
		if(time1[T1] < 3000)
			playFromFile(saveUp, angle, time);
		else
			// Played song will be saved to that slot
	}

	if(getButtonPress(buttonLeft))
	{
		while(getButtonPress(buttonLeft))
		{}
		if(time1[T1] < 3000)
			playFromFile(saveLeft, angle, time);
		else
			// Played song will be saved to that slot
	}

	if(getButtonPress(buttonDown))
	{
		while(getButtonPress(buttonDown))
		{}
		if(time1[T1] < 3000)
			playFromFile(saveDown, angle, time);
		else
			// Played song will be saved to that slot
	}

	if(getButtonPress(buttonRight))
	{
		while(getButtonPress(buttonRight))
		{}
		if(time1[T1] < 3000)
			playFromFile(saveRight, angle, time);
		else
			// Played song will be saved to that slot
	}
}

void playOrSave(ifstream & saveUp, ifstream & saveLeft, ifstream & saveDown, ifstream & saveRight, int angle[COMBOS], int time[COMBOS], tempStoredValues[MAX_NOTES])
{
	while(!getButtonPress(buttonAny))
	{}
	clearTimer(T1);

	if(getButtonPress(buttonEnter))
	{
		while(getButtonPress(buttonEnter))
		{}
		if(time1[T1] < 3000)
		{
			// Activate suck roller. Read and store note values.
			motor[motorA] = 10; // replace motorA with whatever motor we use
			readAndStore(tempStoredValues, colour1, colour2);
			motor[motorA] = 0;

			// Play the stored notes.
			playFromReader(tempStoredValues, angle, time);
		}
		else
			// Shutdown procedure
	}
	else
		buttonOptions(saveUp, saveLeft, saveRight, saveDown, saveRight, angle, time, tempStoredValues);
}

void recalibrate()
{
	motor[motorA] = -10; // whatever motor we using
	while(SensorValue[S3] == 0) // change S3 to whatever sensor is plugged in
	{}
	nMotorEncoder[motorA] = 0;
	motor[motorA] = 10;
	while(nMotorEncoder[motorA] != xxxx) // xxxx is the angle to get to the leftmost note
	{}
	nMotorEncoder[motorA] = 0;
	motor[motorA] = 0;
}

void displayMenu()
{
	displayString(1,?Hold up/down/left/right buttons to save the song.?);
	displayString(2,Press up/down/left/right buttons to play saved music.?);
	displayString(3,?Press enter button to play music from sheet music.?);
	displayString(4,?To exit, hold the enter button.?);
	wait1Msec(5000);
	eraseDisplay();
}

void shutdownProcedure() // Need to figure this part out.
{
	eraseDisplay();
	displayBigTextLine(5,"Skee-yee");
	wait1Msec(5000); //Will end the program
}

task main()
{
	ifstream saveUp("saveUp.txt"); // Need to check how file saving works with robot controllers - video on learn
	ifstream saveLeft("saveLeft.txt");
	ifstream saveDown("saveDown.txt");
	ifstream saveRight("saveRight.txt");
	// check for file open

	int colour1[COMBOS] = {}; // to change
	int colour2[COMBOS] = {}; // to change
	int tempStoredValues[MAX_NOTES] = {}; // to change. a noteValue of 0 should correspond to white-white.
	int angle[COMBOS] = {}; // set these based on testing
	int timeHold[COMBOS] = {}; // to change

	configureAllSensors();
	recalibrate;
	displayMenu;

	while(true) // FIX
	{
		playOrSave(saveUp, saveLeft, saveDown, saveRight, angle, time, tempStoredValues);
		recalibrate;
		displayMenu;
	}



	saveUp.close(); // Close files and exit program.
	saveLeft.close();
	saveDown.close();
	saveRight.close();
}
