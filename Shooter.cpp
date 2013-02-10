#include "Shooter.h"
#include "Portnums.h"
#include <cmath> //So we can use an absolute value function
float totalSpoolUpTime = 2.0;
float tiltStopDistance = 1.0;
float tiltSpeed = 1.0;
float servoPush = 0.10;
float servoPull = 1.0;
float shootSpeedFactor = -1.0;

Shooter::Shooter(int shootIn, int tiltIn, int tachPortIn){
	
	Initialized = false;
	shootJag = new CANJaguar(shootIn);
	tiltJag = new CANJaguar(tiltIn);
	spoolUpTimer = new Timer();
	anglePot = new AnalogChannel(AnglePotPort);
	shootServo = new Servo(ShooterServoPort);
	shootTimer = new Timer();
    tachIn = new DigitalInput(tachPortIn);
    tach = new Counter(tachIn);
}

bool Shooter::Init(){ //Resetting the timer used for spooling up the shooter
	spoolUpTimer->Reset();
    tach->Start();
    tach->Reset();
	Initialized = true;
	return Initialized;
}

void Shooter::StartShooter(){ //Look in idle for how the shooter actually spools up
	spoolUpTimer->Start();
	StartingShooter = true;
}

void Shooter::StopShooter(){
	shootJag->Set(0);
	spoolUpTimer->Reset();
	StartingShooter = false;
}

void Shooter::SetAngle(float desiredAngle){
	//Scale angles to match
	float currentAngle = GetAngle();
	float motorDirection;
	if(desiredAngle < currentAngle){ //Finds which direction the tilt motor needs to run
		motorDirection = -1.0;
	}
	else{
		motorDirection = 1.0;
	}
	if(abs(desiredAngle - currentAngle) < tiltStopDistance){ //Determines when to stop the motor based on the "slop" value
		ManualTilt(0);
	}
	else{
		ManualTilt(tiltSpeed*motorDirection);
	}
}

void Shooter::ManualTilt(float power){
	tiltJag->Set(power);
}

void Shooter::Shoot(){
	shootServo->Set(servoPush);
	shootTimer->Start(); // Where's the state that says we're doing a shoot?
}

float Shooter::GetAngle(){
	float Angle = anglePot->GetValue();
	//Place mapper in this function
	return Angle;
}

void Shooter::Disable(){
	StopShooter();
	spoolUpTimer->Reset();
}

void Shooter::Idle(){
    //Settings.Get
    //float servoPush = 0.25;
    //float servoPull = 0.80;

	double spoolUpTime = spoolUpTimer->Get(); //This is where the action happens for the shooter starting
	double shootTime = shootTimer->Get();
	if(StartingShooter){
		if(spoolUpTime < totalSpoolUpTime){
			shootJag->Set(shootSpeedFactor*(spoolUpTime/totalSpoolUpTime));
		}
		else{
			spoolUpTimer->Stop();
			shootJag->Set(shootSpeedFactor);
		}
	}
	if(shootTime > 0.5){ //The timing for the servo feeding frisbees into the shooter
		shootServo->Set(servoPull);
		shootTimer->Reset();
		shootTimer->Stop();
	}
}

float Shooter::GetRPM() {
    SmartDashboard::PutBoolean("Tach curVal", tachIn->Get());
    SmartDashboard::PutNumber("ShooterTachNum", tach->Get());
    return 0.5/(tach->GetPeriod());
}
