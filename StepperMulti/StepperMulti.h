// ensure this library description is only included once
#ifndef StepperMulti_h
#define StepperMulti_h

// library interface description
class StepperMulti
{
  public:
    // constructors:
    StepperMulti(int number_of_steps, int motor_pin_1, int motor_pin_2);
    StepperMulti(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4);

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void setStep(int number_of_steps);

  	//'loop()'안에서 이 메소드를 호출해야 한다.
  	void moveStep();
    void speedChange();
  private:
    void stepMotor(int this_step);

    int direction;        // Direction of rotation
    int speed;          // Speed in RPMs
    unsigned long step_delay;    // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;        // whether you're driving the motor with 2 or 4 pins
    int step_number;        // which step the motor is on

    // motor pin numbers:
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;

    unsigned long last_step_time;      // time stamp in ms of when the last step was taken

	//스텝 저장
	int steps_left;

  int original_steps;
  long original_speed;
};

#endif
