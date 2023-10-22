import processing.serial.*; // 프로세싱 프로그램의 시리얼 통신 라이브러리 사용
Serial port; // 통신에서 port 변수 사용.

int x=0, y=0, oldx, oldy; // 마우스 좌표 저장 변수 
char d = 0, oldd=0; // 거리 변수 

void setup() // 초기화 함수. 한번만 실행된다.
{
  printArray(Serial.list()); // 하단의 터미널에 시리얼 통신이 가능한 목록을 띄운다.
  port = new Serial(this,"COM5",9600); 
  // 컴퓨터의 COM5로 연결된 포트에 보드레이트 9600으로 접속한다.
  size(250,250); // 창 크기 250x250으로 설정
  smooth(); // 프로세싱 창에서 도형들이 계단형식으로 깨져 나오는 것 방지
}

void draw() {

  oldx = x;  // 원 예전 x 좌표 저장
  oldy = y;  // 원 예전 y 좌표 저장
  oldd = d;
  x = mouseX;  // 마우스 x 좌표 얻음 
  y = mouseY;  // 마우스 y 좌표 얻음
  
  port.write((byte) 0); // 통신의 순서를 위한 0 송신
  port.write((byte) (x)-125); // 마우스 x 좌표 송신
  port.write((byte) (y)-125); // 마우스 y 좌표 송신
  
  d = (char) port.read(); // 초음파 센서 신호 읽기
  
  // 마우스 포인터 좌표 범위 검사하여 벗어나면 창 내부 좌표로 설정
  if(x < 25) 
    x = 25;
  else if(x > 500-25)
    x = 500-25;
    
  if(y < 25) 
    y = 25;
  else if(y > 500-25)
    y = 500-25;   

  // 원 내부의 색을 결정하기 위한 함수
  if(d < 6)
    fill(d*50,255-d*40,d*20);
   else
    fill(255-d*13,d*20,255-d*5);
    
  // 예전 좌표와 비교하여 다른 경우에만 그린다. 
  if(oldx != x || oldy != y || d != oldd) {
    BackgroundScreen();        // 배경 지우기
    ellipse(x, y, 50, 50); // 원 그리기
  }
  
}

void BackgroundScreen() { // 격자무늬 생성
  background(255);
  stroke(150);
  for(int i=0; i< width; i+=10){ // 가로로 10픽셀 간격마다 생성
    line(i,0,i,height);
  }
  for(int i=0; i< height; i+=10){ // 세로로 10픽셀 간격마다 생성
    line(0,i,width,i);
  }
}
