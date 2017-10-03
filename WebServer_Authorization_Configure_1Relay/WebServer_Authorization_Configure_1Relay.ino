#include <SPI.h>
#include <Ethernet2.h>
#include <TextFinder.h>
// Incluye API similar a EEPROM for FlashStorage
#include <FlashAsEEPROM.h>
// # include <avr / wdt.h>

byte IP1 = 10; // la primera parte de la dirección IP
byte IP2 = 2; // la segunda parte de la dirección IP
byte IP3 = 20; // la parte thirth de la dirección IP
byte IP4 = 250; // la cuarta parte de la dirección IP
byte GW1 = 10; // la primera parte de la PISTA // Tu ip de Puerta de Entrada
byte GW2 = 2; // la segunda parte de la GATEWAY
byte GW3 = 20; // la cuarta parte de la PISTA
byte GW4 = 1; // la cuarta parte de la GATEWAY
byte SUB1 = 255; // primera parte de la SUBNETMASK
byte SUB2 = 255; // la segunda parte de la SUBNETMASK
byte SUB3 = 255; // la parte thirth del SUBNETMASK
byte SUB4 = 0; // la cuarta parte de la SUBNETMASK
byte SET = 0;

// Configurar el EthernetShield
byte mac [] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip [] = {IP1, IP2, IP3, IP4};
byte gateway [] = {GW1, GW2, GW3, GW4};
byte subnet [] = {SUB1, SUB2, SUB3, SUB4};
EthernetServer server(80);

const int relay = 6; // Pin del relay
String estado = "Encendido"; // Estado del Relay inicialmente "OFF"

const int led1 = 11;
const int led2 = 9;

byte ID = 0x95; // usado for identificar si los datos válidos en EEPROM son el bit "know", // 0x95
// si esto se escribe en EEPROM el esbozo ha corrido antes
// Usamos esto, de modo que la primera vez que ejecute este boceto usará
// los valores escritos arriba. 
// definición de la dirección EEPROM que estamos utilizando for qué datos
const byte ID_ADDR = 0; // la dirección de EEPROM usada for almacenar el ID (for comprobar si el esbozo ya ha corrido)
const byte IP_ADDR1 = 1; // la dirección EEPROM utilizada for almacenar la primera parte de la dirección IP
const byte IP_ADDR2 = 2; // la dirección EEPROM utilizada for almacenar la segunda parte de la dirección IP
const byte IP_ADDR3 = 3; // la dirección EEPROM utilizada for almacenar la parte temporal de la dirección IP
const byte IP_ADDR4 = 4; // la dirección EEPROM utilizada for almacenar la cuarta parte de la dirección IP
const byte GW_ADDR1 = 5; // la dirección EEPROM utilizada for almacenar la primera parte de la GATEWAY
const byte GW_ADDR2 = 6; // la dirección EEPROM utilizada for almacenar la segunda parte de la GATEWAY
const byte GW_ADDR3 = 7; // la dirección EEPROM utilizada for almacenar la parte temporal de la PÁGINA
const byte GW_ADDR4 = 8; // la dirección EEPROM utilizada for almacenar la cuarta parte de la GATEWAY
const byte SUB_ADDR1 = 9; // la dirección EEPROM utilizada for almacenar la primera parte de la SUBNETMASK
const byte SUB_ADDR2 = 10; // la dirección EEPROM utilizada for almacenar la segunda parte de la SUBNETMASK
const byte SUB_ADDR3 = 11; // la dirección EEPROM utilizada for almacenar la parte temporal de la SUBNETMASK
const byte SUB_ADDR4 = 12; // la dirección EEPROM utilizada for almacenar la cuarta parte de la SUBNETMASK
byte ip_addr [] = {IP_ADDR1, IP_ADDR2, IP_ADDR3, IP_ADDR4};
byte gw_addr [] = {GW_ADDR1, GW_ADDR2, GW_ADDR3, GW_ADDR4};
byte sub_addr [] = {SUB_ADDR1, SUB_ADDR2, SUB_ADDR3, SUB_ADDR4};

void (* resetea) (void) = 0; // resetea el arduino, Parecido a reset_cpu (). C compilador de imágenes. pero no resetea del todo !!!

boolean buttonState = 0;
boolean pasa = 0;
int configuration=0;

int activa_reset=0;
long tiempo=0;


void setup ()
{
  //Serial.begin(9600);
  // while (! Serial) {}

  pinMode (12, INPUT);

  pinMode (relay, OUTPUT); // establece el pin digital como salida
  pinMode (led1, OUTPUT); // establece el pin digital como salida
  pinMode (led2, OUTPUT); // establece el pin digital como salida
  
  // digitalWrite (relay, ALTO); // Evita que los relays se enciendan
  digitalWrite (led1, LOW); // Evita que los relays se enciendan
  digitalWrite (led2, HIGH); // Evita que los relays se enciendan
  
  ShieldSetup (); // Configurar el escudo Ethernet
  
  server.begin (); // inicio del servidor
  delay (1);
  /*Serial.print("ready on ip: ");
  Serial.print (IP1, DEC);
  Serial.print (".");
  Serial.print (IP2, DEC);
  Serial.print (".");
  Serial.print (IP3, DEC);
  Serial.print (".");
  Serial.println (IP4, DEC); */
}

void ShieldSetup ()
{
  int idcheck = EEPROM.read (ID_ADDR);
  //Serial.println(idcheck,DEC);
  delay (1);
  buttonState = digitalRead (12);
  
 
 
  // (idcheck! = ID) ||
  if (buttonState == true) {
    // ifcheck id no es el valor como const byte ID,
    // significa que este boceto NO ha sido usado for configurar el escudo antes
    Ethernet.begin (mac, ip, gateway, subnet); // simplemente usa los valores escritos al principio del boceto
    //Serial.println( "primer boceto de tiempo se ha ejecutado");
    configuration = 1;    
  }
  if ((idcheck == ID) && (buttonState == false)) {
    // si id es el mismo valor que const byte ID,
    // significa que este boceto se ha utilizado for configurar el escudo.
    // Así que leeremos los valores de EERPOM y los usaremos
    // for configurar el escudo.
    IP1 = EEPROM.read (IP_ADDR1);
    IP2 = EEPROM.read (IP_ADDR2);
    IP3 = EEPROM.read (IP_ADDR3);
    IP4 = EEPROM.read (IP_ADDR4);
    GW1 = EEPROM.read (GW_ADDR1);
    GW2 = EEPROM.read (GW_ADDR2);
    GW3 = EEPROM.read (GW_ADDR3);
    GW4 = EEPROM.read (GW_ADDR4);
    SUB1 = EEPROM.read (SUB_ADDR1);
    SUB2 = EEPROM.read (SUB_ADDR2);
    SUB3 = EEPROM.read (SUB_ADDR3);
    SUB4 = EEPROM.read (SUB_ADDR4);
    byte ip [] = {IP1, IP2, IP3, IP4};
    byte gateway [] = {GW1, GW2, GW3, GW4};
    byte subnet [] = {SUB1, SUB2, SUB3, SUB4};
    Ethernet.begin (mac, ip, gateway, subnet);
    //Serial.println("NOT el primer boceto se ha ejecutado ");
    configuration = 2;
  }
}


void ShieldValueWrite ()
{
  //Serial.println( "escribir en EEPROM");
  for (int i = 0; i <4; i ++)
  {
    EEPROM.write (ip_addr [i], ip [i]);
  }
  for (int i = 0; i <4; i ++)
  {
    EEPROM.write (gw_addr [i], gateway [i]);
  }
  for (int i = 0; i <4; i ++)
  {
    EEPROM.write (sub_addr [i], subnet [i]);
  }  
}
///////////////////////////////// relay //////////////// ///////////////////////////////////////////
void SendOKpage (EthernetClient &client)
{
         // Enviamos al cliente una respuesta HTTP
            client.println ("HTTP / 1.1 200 OK");
            client.println ("Content-Type: text / html");
            client.println ("Connnection: close");
            client.println ();
 
            // Pagina web en formato HTML
            client.println ("<! DOCTYPE HTML>");
            client.println ("<html>");
              // agrega una etiqueta meta refresh, por lo que el navegador vuelve a tirar cada 5 segundos:
            //client.println("<meta http-equiv = \ "refresh \" content = \ "5 \"> ");
            client.println("<head>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1 align='center'>Altec</h1><h3 align='center'>Control Relay Servidor Web</h3>");
            //Creamos los botones. Para enviar parametros a traves de HTML se utiliza el metodo URL encode. Los parametros se envian a traves del simbolo '?'
            client.println("<div style='text-align:center;'>");
            /*client.println("<button onClick=location.href='./?Relay=Reset\' style='margin:auto;background-color: #94B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("Reset");
            client.println("</button>");*/
            
            client.println("<button onClick=location.href='./?Relay=ON\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("ON");
            client.println("</button>");
            client.println("<button onClick=location.href='./?Relay=OFF\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("OFF");
            client.println("</button>");
            
            client.println("<br /><br />");
            client.println("<b>Estado de Equipo = ");
            client.print(estado);
            client.println("</b><br />");
            client.println("</b></body>");
            client.println("</html>");

                  
}

void SendAuthentificationpage(EthernetClient &client)
{
          client.println("HTTP/1.1 401 Authorization Required");
          client.println("WWW-Authenticate: Basic realm=\"Secure Area\"");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<HTML>  <HEAD>   <TITLE>Error</TITLE>");
          client.println(" </HEAD> <BODY><H1>401 Unauthorized.</H1></BODY> </HTML>");
}

char linebuf[80];
int charcount=0;
boolean authentificated=false;


///////////////////////////////////// FIN Relay /////////// /////////////////////////////////////////

void loop ()
{
  if (configuration==1)
  { 
  EthernetClient client = server.available();
  if (client) {
    TextFinder  finder(client );
    while (client.connected()) {      
      if (client.available()) {          
        if( finder.find("GET /") ) {
          //Serial.println("connection has been made");
          if (finder.findUntil("SBM", "\n\r")){
            //Serial.println();
            //Serial.println("found a submitted form");
            SET = finder.getValue();
              while(finder.findUntil("DT", "\n\r")){
                int val = finder.getValue(); 
                if(val >= 0 && val <= 3) {
                  ip[val] = finder.getValue();
                }
                if(val >= 4 && val <= 7) {
                  subnet[val - 4] = finder.getValue();
                }
                if(val >= 8 && val <= 11) {
                  gateway[val - 8] = finder.getValue();
                }
              }
            /*Serial.print("The new IP address is:");
            Serial.print(ip[0],DEC);
            for (int i= 1; i < 4; i++){
              Serial.print(".");
              Serial.print(ip[i],DEC);
            }
            Serial.println();
            Serial.print("The new Subnet is:");
            Serial.print(subnet[0],DEC);
            for (int i= 1; i < 4; i++){
              Serial.print(".");
              Serial.print(subnet[i],DEC);
            }
            Serial.println();
            Serial.print("The new gateway is:");
            Serial.print(gateway[0],DEC);
            for (int i= 1; i < 4; i++){
              Serial.print(".");
              Serial.print(gateway[i],DEC);
            }
            Serial.println();
            */
            ShieldValueWrite(); // writing all the values whitin the form into EEPROM
            EEPROM.write(ID_ADDR, 0x95); //set ID to the known bit, so when you reset the Arduino is will use the EEPROM values
            //Serial.println("all data has been written to EEPROM");
            //Serial.println("you can now reset the Arduino");
            //Serial.println("and use the new ip in your browser");
            EEPROM.commit();
            //Serial.println("Done!");
            resetea();
            //
            // if al the data has been written to EEPROM we should reset the arduino
            // for now you'll have to use the hardware reset button
          }
          }           
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.print("<html><body><table><form><input type=\"hidden\" name=\"SBM\" value=\"1\">");
          client.print("<tr><td>IP: <input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT0\" value=\"");
          client.print(IP1,DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT1\" value=\"");
          client.print(IP2,DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT2\" value=\"");
          client.print(IP3,DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT3\" value=\"");
          client.print(IP4,DEC);
          client.print("\"></td></tr><tr><td>MASK: <input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT4\" value=\"");
          client.print(subnet[0],DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT5\" value=\"");
          client.print(subnet[1],DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT6\" value=\"");
          client.print(subnet[2],DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT7\" value=\"");
          client.print(subnet[3],DEC);
          client.print("\"></td></tr><tr><td>GW: <input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT8\" value=\"");
          client.print(gateway[0],DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT9\" value=\"");
          client.print(gateway[1],DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT10\" value=\"");
          client.print(gateway[2],DEC);
          client.print("\">.<input type=\"text\" size=\"1\" maxlength=\"3\" name=\"DT11\" value=\"");
          client.print(gateway[3],DEC);
          client.print("\"></td></tr><tr><td><input type=\"submit\" value=\"CONFIRMAR\"></td></tr>");
          client.print("</form></table></body></html>");
          
          break;
        }
      }
   // delay(1);
    client.stop();
  }
 }// Fin configuration

 if (configuration==2)
 {
  // listen for incoming clients

  EthernetClient client = server.available();
  if (client) {
  //  Serial.println("new client");
    boolean currentLineIsBlank = true; //Una peticion HTTP acaba con una lÃ­nea en blanco
    String cadena=""; //Creamos una cadena de caracteres vacÃ­a
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    authentificated=false;
    // an http request ends with a blank line
   //delay(1);
    while (client.connected()) {
       //delay(1);
      if (client.available()) {
         //delay(1);
        char c = client.read();
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        
        //Serial.write(c);
        
         cadena.concat(c);//Unimos el String 'cadena' con la peticion HTTP (c). De esta manera convertimos la peticion HTTP a un String
 
         //Ya que hemos convertido la peticion HTTP a una cadena de caracteres, ahora podremos buscar partes del texto.
         int posicion=cadena.indexOf("Relay="); //Guardamos la posicion de la instancia "Relay=" a la variable 'posicion'
 
          if(cadena.substring(posicion)=="Relay=OFF")//Si a la posicion 'posicion' hay "Relay=ON"
          {
            digitalWrite(relay,HIGH);
           digitalWrite(led2,LOW);
            digitalWrite(led1,HIGH);
            estado="Apagado";
           
             //delay(1);
          }
          
          if(cadena.substring(posicion)=="Relay=ON")//Si a la posicion 'posicion' hay "Relay=OFF"
          {
            digitalWrite(relay,LOW);
            digitalWrite(led2,HIGH);
            digitalWrite(led1,LOW);
            estado="Encendido";
            
             //delay(1);
          }
                 
          
         /* if(cadena.substring(posicion)=="Relay=Reset")//Si a la posicion 'posicion' hay "Relay=OFF"
          {
            digitalWrite(relay,HIGH);
            digitalWrite(led2,LOW);
            digitalWrite(led1,HIGH);
            estado="Reseteando";
           
           
          }
          */
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          if (authentificated)
          {
            SendOKpage(client);
             //Serial.println("pasa_por aqui");
             //Serial.println(tiempo);
              //delay(1);
          }
           else
            SendAuthentificationpage(client); 
            //delay(1); 
          break;
        }
        
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"Authorization: Basic")>0 && strstr(linebuf,"Y2hhcmxpZToxMjM0")>0) // usuario = charlie, pwd=1234 , Base64 = encode -> charlie:1234 = Y2hhcmxpZToxMjM0
            authentificated=true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;

        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  //  Serial.println("client disonnected");
  }
 }
}


