
long waitTime = 1000;
byte lastBtStatus = 0; 
bool btLP = 0; 
unsigned long lastChangedTime;
bool editStatus = 0,btDown=0;
int d;
int t[] = {20,15,3};

void setup() {
  pinMode(3, OUTPUT); //A
  pinMode(4, OUTPUT); //B
  pinMode(5, OUTPUT); //C
  pinMode(6, OUTPUT); //D
  pinMode(7, OUTPUT); //E
  pinMode(8, OUTPUT); //F
  pinMode(9, OUTPUT); //G
  pinMode(10, OUTPUT); //ĐÈN ĐỎ
  pinMode(11, OUTPUT); //ĐÈN VÀNG
  pinMode(12, OUTPUT); //ĐÈN XANH
  pinMode (A0, OUTPUT); //DK led số 1
  pinMode (A1, OUTPUT); //DK led số 2
  pinMode (A2, INPUT);
  pinMode (A3, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600); //Bật Serial để debug
  Serial.println("Bat dau");
  digitalWrite(A3, LOW);
  digitalWrite(13, LOW);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(10,HIGH);
}



void loop(){
  
  den();
  if (d==2) d=0;
  else d++;
 
}

void den(){
  switch (d){
    case 1:denXanh();
          d=1;
          dem(t[1]);
          break;
    case 0:denDo();
           d=0;
          dem(t[0]);
          break;
    case 2:denVang();
          d=2;
          dem(t[2]);
          break;
  }
  btDown=0;
}


void denXanh(){
  digitalWrite(11,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(10,HIGH);
}

void denDo(){
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(10,LOW);
}

void denVang(){
  digitalWrite(11,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(10,HIGH);
}

byte checkbt(){
  btDown=0;
  bool reading = !digitalRead(2); // đọc giá trị của button
  Serial.print(reading);
  if (lastBtStatus == 2 && reading){
    return 0;
  }
  if (lastBtStatus == 2 && !reading){
    lastBtStatus = 0 ;
    btLP=0;
    return 0;
  }
  if (reading != lastBtStatus) { // Nếu bạn đang nhấn button rồi thả ra giữa chừng hoặc đang thả button rồi nhấn lại 
    
    if ((!reading) && (lastBtStatus)) {
      btDown=1; 
      lastBtStatus = reading; //Cập nhập trạng thái cuối cùng.
      lastChangedTime = millis(); //Cập nhập thời gi
      Serial.print("Down");
      return 1;
    }
    lastBtStatus = reading; //Cập nhập trạng thái cuối cùng.
    lastChangedTime = millis(); //Cập nhập thời gi

  } // Còn nếu bạn đang nhấn giữ button hoặc thả nó thời gian dài chỉ sẽ không ảnh hưởng đến việc này
  
  if (millis() - lastChangedTime > waitTime) { // Nếu sự chênh lệch giữa thời điểm đang xét và thời điểm cuối cùng thay đổi trạng thái của button lớn hơn thời gian đợi để tạo sự kiện nhấn giữ THÌ nó là một sự kiến nhấn giữ
    btLP = reading; // Cập nhập trạng thái của button = với trạng thái của button
    lastChangedTime = millis();
    lastBtStatus=2;
  }
  if (btLP){
    Serial.print("Press");
    return 2;
  }
  return 0;
}



void dem(byte so){
  for (int i=so;i>=0;i--) {
    for (int j=1;j<=50;j++){
        if ((i/10)!=0) {
          inSo(i/10);
          analogWrite(A0,130);
          delay(4);
          analogWrite(A0,0);
        } else delay(4);
        inSo(i%10);
        analogWrite(A1,130);
        delay(4);
        analogWrite(A1,0);
        switch (checkbt()){
          case 1: return;
          case 2: 
                  nhay();
                  den();
                  return;
        }
//        if (checkbt()==2){
//          nhay();
//          return;
//        } 
    }
  }
}

void nhay(){
  Serial.print("Nhay");
    while(true){
    for (int i=1;i<=25;i++){
      inSo(map(analogRead(A2),0,1023,0,50)/10);
      analogWrite(A0,130);
      
//      if (checkbt()==2) {
//          t[d]=map(analogRead(A2),0,1023,0,50);
//          analogWrite(A1,0);
//          analogWrite(A0,0);
//          return;
//      }
      switch (checkbt()){
        case 1: {
          switch (d){
            case 0: denXanh();
                     d=1;
                     break;
            case 1: denVang();
                    d=2;
                    break;
            case 2: denDo();
                    d=0;
                    break;
          }
          break;
        }
        case 2:
        t[d]=map(analogRead(A2),0,1023,0,50);
        analogWrite(A1,0);
        analogWrite(A0,0);
        return;
      }
      delay(10);
      analogWrite(A0,0);
    }
    for (int i=1;i<=25;i++){
      inSo(map(analogRead(A2),0,1023,0,50)/10);
      analogWrite(A0,130);
      delay(5);
      analogWrite(A0,0);
      inSo(map(analogRead(A2),0,1023,0,50)%10);
      analogWrite(A1,130);
//      if (checkbt()==2) {
//        
//      }
      switch (checkbt()){
        case 1: {
          switch (d){
            case 0: denXanh();
                     d=1;
                     break;
            case 1: denVang();
                    d=2;
                    break;
            case 2: denDo();
                    d=0;
                    break;
          }
          break;
        }
        case 2:
        t[d]=map(analogRead(A2),0,1023,0,50);
        analogWrite(A1,0);
        analogWrite(A0,0);
        return;
      }
      delay(5);
      analogWrite(A1,0);
    }
  }
}
void xoa(){
  digitalWrite(9, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

void inSo(byte number){
  switch (number){
    case 0:{
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      break;}
    case 1:{
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      break;}
    case 2:{
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      break;}
    case 3:{
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      break;}
    case 4:{
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;}
    case 5:{
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;}
    case 6:{
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;}
    case 7:{
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
      break;}
    case 8:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;
    case 9:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;
  }
}
