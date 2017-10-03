Web Server En Arduino M0 
Feather-M0-Ethernet2-FeatherWing 

Rele Controlado Por Internet, IP Configurable. 
¡Se pueden poner mas Reles En varias Salidas!...
  
Con Usuario y Clave de Acceso.

Base64 = Encode -> charlie:1234 =  Y2hhcmxpZToxMjM0.

Funcionamiento: Para Configurar nueva IP,GW,SUB

Cuando Enciendo el Feather-M0, pulsado el boton en el Pin 12, 
entro en modo Configuracion,

Por Default: Direccion Precargada .
Direccion IP :10.2.20.250
Gateway : 10.2.20.1 // Poner La que corresponda (Gateway).
Subnetmask : 255.255.255.0

Escribo nueva IP,GW,SUB, Guardo y Listo.

Apago mi Feather M0 enciendo sin pulsar boton (Pin12) 
entra en la nueva Direccion Ip, GW, SUB que configure anteriormente. 

Salida del pin del Rele = Pin 6
Led de Encendido y Apagado = Pin 11 y 9 .

Uso de Librerias para Feather M0

Ethernet2.h // Modificada para que no se cuelgue con el paso del tiempo.

TextFinder.h //

FlashAsEEPROM.h // Usada como EEPROM en Feather M0 Para Guaradar los datos.

