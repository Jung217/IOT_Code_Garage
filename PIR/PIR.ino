#define BLINK_GPIO (gpio_num_t) 2
#define inPin 17  //HC-SR501人體紅外線感測器OUT信號腳 連接到ESP32 GPIO17 
int val;

void setup()
{
  Serial.begin(9600);
  gpio_pad_select_gpio(BLINK_GPIO);  // 選定要控制的接腳
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  pinMode(inPin,INPUT);    //設置inPin對應的腳GPIO17為輸入
}
void loop()
{
  val=0;
  val=digitalRead(inPin); //人體紅外線感測器讀出數位值
  Serial.println(val);
  if(val==HIGH) // 如果有人在移動
  { 
    Serial.println("Y");
    gpio_set_level(BLINK_GPIO, 1); //有源蜂鳴器發出聲響
  }
  else
  { 
    Serial.println("N");
    gpio_set_level(BLINK_GPIO, 0); //有源蜂鳴器無聲
  }
  delay(500);
}

