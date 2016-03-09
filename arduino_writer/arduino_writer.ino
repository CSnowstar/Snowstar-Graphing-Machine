//
typedef unsigned int uint;
typedef unsigned long ulong;

class Stepper{
public:
    Stepper(double stepsPerRevolution, int pin0, int pin1, int pin2, int pin3, ulong delay, bool reverse){
        _stepsPerRevolution = stepsPerRevolution;
        _pin0 = pin0;
        _pin1 = pin1;
        _pin2 = pin2;
        _pin3 = pin3;

        pinMode(_pin0, OUTPUT);
        pinMode(_pin1, OUTPUT);
        pinMode(_pin2, OUTPUT);
        pinMode(_pin3, OUTPUT);
        _reverse = reverse;
        _delay_micros = delay;
    }
    void Revolve(double revolutions){
        _left_steps += revolutions * _stepsPerRevolution;
    }
    bool my_turn(){
        ulong now_micros = micros();
        if(now_micros - _last_micros < _delay_micros){
            return false;
        }
        //
        if(_left_steps == 0)
            return false;
        if((_left_steps > 0) ^ _reverse)
            _step--;
        else
            _step++;
        if(_left_steps > 0)
            _left_steps--;
        else
            _left_steps++;
        _step %= 8;
        uint pins;
        if(0 == (_step % 2))
            pins = 1 << (_step / 2);
        else
            pins = 11 << (_step / 2);

        digitalWrite(_pin0, (pins >> 0) & 1 );
        digitalWrite(_pin1, (pins >> 1) & 1 );
        digitalWrite(_pin2, (pins >> 2) & 1 );
        digitalWrite(_pin3, (pins >> 3) & 1 );
        //
        _last_micros = now_micros;
        return true;
    }
    bool done(){
        return _left_steps == 0;
    }
private:
    // const
    int _stepsPerRevolution;
    int _pin0;
    int _pin1;
    int _pin2;
    int _pin3;
    bool _reverse;
    ulong _delay_micros;
    // var
    ulong _last_micros = 0;
    ulong _last_millis = 0;
    uint _step = 0;
    int _left_steps = 0;
    const double pi = 3.1415926535;
    const double e = 2.71828;
};

class Servo{
public:
    Servo(int pin, bool reverse){
        _pin = pin;
        _reverse = reverse;
        pinMode(_pin, OUTPUT);
    }
    void RotateTo(float percent){
        _percent = percent;
        if(_reverse){
            analogWrite(_pin, 255 * _percent);
        }else{
            analogWrite(_pin, 255 - 255 * _percent);
        }
    } 
private:
    int _pin;
    bool _reverse;
    int _percent;
};

// 28BYJ
Stepper stepperL(4075.776, 10, 11, 12, 13, 1800, false);
// 28BYJ
Stepper stepperR(4075.776, 4, 5, 6, 7, 1800, true);
// SG90
Servo   servoPen(3, false);
// Led
//Blinker blinkLed(9, false);
const int pinBlink = 9;

struct Action{
    float revolutionsL;
    float revolutionsR;
    float rotatePen;
};

// 脉冲交替式半双工串口异步实时通信...
bool serial_command_turn(){
    if(!Serial.available())
        return false;
    Action act;
    int c = Serial.readBytes((char*)&act, sizeof(act));
    if(c != sizeof(act))
        return false;

    stepperL.Revolve(act.revolutionsL);
    stepperR.Revolve(act.revolutionsR);
    servoPen.RotateTo(act.rotatePen);
    return true;
}


bool blink_turn(){
    ulong x  = millis() % 1000;
    if(x > 500){
        analogWrite(pinBlink, 255);
    }else{
        analogWrite(pinBlink, 0);
    }
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    //
    pinMode(pinBlink, OUTPUT);
}
void loop()
{
    blink_turn();
    serial_command_turn();
    stepperL.my_turn();
    stepperR.my_turn();
}





