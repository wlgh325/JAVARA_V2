#include "Arduino.h"
#include "StepperMulti.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
StepperMulti::StepperMulti(int number_of_steps, int motor_pin_1, int motor_pin_2)
{
  this->steps_left = 0;	//스탭초기화

  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);

  // When there are only 2 pins, set the other two to 0:
  this->motor_pin_3 = 0;
  this->motor_pin_4 = 0;

  // pin_count is used by the stepMotor() method:
  this->pin_count = 2;
}


/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

StepperMulti::StepperMulti(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4)
{
  this->steps_left = 0;	//스탭초기화

  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  pinMode(this->motor_pin_3, OUTPUT);
  pinMode(this->motor_pin_4, OUTPUT);

  // pin_count is used by the stepMotor() method:
  this->pin_count = 4;
}

/*
  Sets the speed in revs per minute

*/
void StepperMulti::setSpeed(long whatSpeed)
{
  this->original_speed = whatSpeed;
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
}

/*
  Moves the motor steps_to_move steps.  If the number is negative,
   the motor moves in the reverse direction.
 */
void StepperMulti::setStep(int steps_to_move)
{
  this->steps_left = abs(steps_to_move);  // how many steps to take
  this->original_steps = abs(steps_to_move);

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) {this->direction = 1;}
  if (steps_to_move < 0) {this->direction = 0;}

  //스탭을 진행 한다.
  moveStep();
}

void StepperMulti::moveStep()
{
	//남은 스텝이 있는지?
	if(this->steps_left > 0)
	{
    speedChange();
    unsigned long now = micros();
		// move only if the appropriate delay has passed:
		if (now - this->last_step_time >= this->step_delay)
		{
			// get the timeStamp of when you stepped:
			this->last_step_time = now;
			// increment or decrement the step number,
			// depending on direction:
			if (this->direction == 1)
			{
				this->step_number++;
				if (this->step_number == this->number_of_steps)
				{
					this->step_number = 0;
				}
			}
			else
			{
				if (this->step_number == 0)
				{
					this->step_number = this->number_of_steps;
				}
				this->step_number--;
			}
			// decrement the steps left:
			this->steps_left--;
			// step the motor to step number 0, 1, 2, or 3:
			stepMotor(this->step_number % 4);
		}
	}
}


/*
 * Moves the motor forward or backwards.
 */
void StepperMulti::stepMotor(int thisStep)
{
  if (this->pin_count == 2) {
    switch (thisStep) {
      case 0: /* 01 */
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
      break;
      case 1: /* 11 */
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, HIGH);
      break;
      case 2: /* 10 */
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, LOW);
      break;
      case 3: /* 00 */
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, LOW);
      break;
    }
  }
  if (this->pin_count == 4) {
    switch (thisStep) {
      case 0:    // 1010
        digitalWrite(motor_pin_1, HIGH);
        digitalWrite(motor_pin_2, LOW);
        digitalWrite(motor_pin_3, HIGH);
        digitalWrite(motor_pin_4, LOW);
      break;
      case 1:    // 0110
        digitalWrite(motor_pin_1, LOW);
        digitalWrite(motor_pin_2, HIGH);
        digitalWrite(motor_pin_3, HIGH);
        digitalWrite(motor_pin_4, LOW);
      break;
      case 2:    //0101
        digitalWrite(motor_pin_1, LOW);
        digitalWrite(motor_pin_2, HIGH);
        digitalWrite(motor_pin_3, LOW);
        digitalWrite(motor_pin_4, HIGH);
      break;
      case 3:    //1001
        digitalWrite(motor_pin_1, HIGH);
        digitalWrite(motor_pin_2, LOW);
        digitalWrite(motor_pin_3, LOW);
        digitalWrite(motor_pin_4, HIGH);
      break;
    }
  }
}

void StepperMulti::speedChange(){
  int smallStep = 300;  //about 30 degree
  long smallSpeed = 0;

  if(this->steps_left < smallStep){
    smallSpeed = (long)( (this->steps_left / 100 ) + 4);
    this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / smallSpeed;
  }
  else if( (this->steps_left * 1.0 / this->original_steps) >= 0.75){
    smallSpeed = (long)( (  (this->original_steps - this->steps_left) / 100 ) + 4);
    this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / smallSpeed;
  }
  else{
    this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / this->original_speed;
  }

}
