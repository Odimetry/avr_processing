import processing.serial.*; // import serial library
Serial port; // define "port" variable

int x=0, y=0, oldx, oldy; // mouse cordinate
char d = 0, oldd=0; // distance variable

void setup()
{
  printArray(Serial.list()); // print available serial ports
  port = new Serial(this,"COM5",9600); 
  // connect to "COM5" with baudrate 9600
  size(250,250); // fix window size 250px x 250px
  smooth(); // Antialiasing
}

void draw() { // Loop function

  oldx = x;  // store old circle's x
  oldy = y;  // store old circle's y
  oldd = d;
  x = mouseX;  // Get mouse X
  y = mouseY;  // Get mouse Y

  // restrict the range of mouse cursor
  if(x < 25) 
    x = 25;
  else if(x > 500-25)
    x = 500-25;
  if(y < 25) 
    y = 25;
  else if(y > 500-25)
    y = 500-25;  

  /*
  ///////////
  The packet is
  0 - x cordinate - y cordinate
  ///////////
  */
  
  port.write((byte) 0); // Start of packet
  port.write((byte) (x)-125); // transmit mouse X cordinate
  port.write((byte) (y)-125); // transmit mouse Y cordinate
  
  d = (char) port.read(); // receive ultrasonic data
  
  // define color of color
  if(d < 6)
    fill(d*50,255-d*40,d*20);
   else
    fill(255-d*13,d*20,255-d*5);
    
  // Redraw only when cordinate changed or data changed
  if(oldx != x || oldy != y || d != oldd) {
    BackgroundScreen(); // redraw backgroundscreen
    ellipse(x, y, 50, 50); // draw circle
  }
  
}

void BackgroundScreen() { // draw plaid on background
  background(255); // white
  stroke(150);
  for(int i=0; i< width; i+=10){ // draw horizontal line each 10px
    line(i,0,i,height);
  }
  for(int i=0; i< height; i+=10){ // draw vertical line each 10px
    line(0,i,width,i);
  }
}
