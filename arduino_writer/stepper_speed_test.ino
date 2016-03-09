//
typedef unsigned int uint;
typedef unsigned long ulong;

uint _step = 0;
void step(){
	_step++;

    _step %= 8;
    uint pins;
    if(0 == (_step % 2))
        pins = 1 << (_step / 2);
    else
        pins = 11 << (_step / 2);

    digitalWrite(4, (pins >> 0) & 1 );
    digitalWrite(5, (pins >> 1) & 1 );
    digitalWrite(6, (pins >> 2) & 1 );
    digitalWrite(7, (pins >> 3) & 1 );
}
void setup()
{
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
}
uint x = 18000;
void loop()
{
    step();
    delayMicroseconds(x/10);
    if(x>=8000){
    	x--;
    }
    if ( x==0){
    	x=18000;
    }
}





