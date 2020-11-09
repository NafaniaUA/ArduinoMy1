#include <SPI.h>
#include <Ethernet.h>
// mac-адреса плати та ip-адреса серверу
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(169,254,51,64);
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
 
int pinLed = 7; // Пін світлодіода
int pinState = 0; //Стан піну
String getData="";
boolean startGet=false;
void setup()
{
Serial.begin(9600);
pinMode(pinLed,OUTPUT); // пін підключення світлодіоду
//digitalWrite(7,LOW); // вимкнути світлодіод

// инициализации библиотеки Ethernet server
Ethernet.begin(mac, ip);
server.begin();
}
void loop()
{
// ожидание подключения клиентов
EthernetClient client = server.available();
if (client)
{
boolean currentLineIsBlank = true;
while (client.connected())
{
if (client.available())
{
char c = client.read();
if(startGet==true) // дані після '?'
getData+=c;
if(c == '?') // почтаок збору даних '?'
startGet=true;
if (c == '\n' && currentLineIsBlank) // закінчення отримання даних
{
if(getData.length()<1) // запит без get-даних
{
pinState=0;

}
else
{
pinState=int(getData[5])-48;

}
// відправлення заголовку клієнту
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");
client.println();
// формування сторінки
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<h3>SMART HOUSE ARDUINO + LED</h3>");
client.println("<form method='get'>");
// світлодіод
client.print("<div>");
client.print("led1 off<input type='radio' name='led1' value=0 onclick='document.getElementById(\"submit\").click();' ");
if (pinState == 0)
client.print("checked");
client.println(">");
client.print("<input type='radio' name='led1' value=1 onclick='document.getElementById(\"submit\").click();' ");
if (pinState == 1)
client.print("checked");
client.println("> on");
client.println("</div>");
client.println("<input type='submit' id='submit' style='visibility:hidden;' value='Refresh'>"); //оновлення сторінки
client.println("</form>");
client.println("</html>");
break;
}
if (c == '\n')
{currentLineIsBlank = true;}
else if (c != '\r')
{currentLineIsBlank = false;}
}
}
}
// затрима для отримання клієнтом даних
delay(1);
// закрити з'єднання
client.stop();
// світлодіоди вимкнути або вімкнути
digitalWrite(pinLed,pinState);
startGet=false;
getData="";
}
