


//Incluimos las librerías/
//#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>



#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
/* Datos de acceso a cuenta. */
#define AUTHOR_EMAIL "juancarlosnerifigueroa@gmail.com"
#define AUTHOR_PASSWORD "ubts htis dsyg pynk"
/* Correo electrónico del recipiente*/
#define RECIPIENT_EMAIL "juan_carlosn@outlook.com"
/* Objeto SMTP para enviar el correo electrónico */
SMTPSession smtp;

int Estado1 = 0;

void setup(){

  Serial.begin(115200);
  Serial.println();
  Serial.print("Conectando a punto de acceso");
  WiFi.begin( "Telcel_B311_46CC", "9d9mE8DqbYH");

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }

  Serial.println("");
  Serial.println("Conectado a WiFi.");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  smtp.debug(1);

  /* Solicita resultados de envió */
  //smtp.callback(smtpCallback);

  /* Configurar datos de sesión */
  ESP_Mail_Session session;

  /* Configurar la sesión */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declara la clase del mensaje */
  SMTP_Message message;

  /* Configura cabecera del mensaje */
  message.sender.name = "Riego";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Alerta";
  message.addRecipient("BY:JCNF", RECIPIENT_EMAIL);
 
  //Manda texto
  String textMsg = "Se a iniciado el sistema correctamente";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Configura cabecera personalizada */

  /* Conecta al servidor */
  if (!smtp.connect(&session))
    return;

  /* Manda correo y cierra sesión */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());

}

void loop(){
   if (Serial.available() > 0) {
    // get incoming byte:
  
  Estado1 =1;}

  if(WiFi.status() == WL_CONNECTED){
   Serial.println("conectado");
   Serial.println(Estado1);
  }
  else{
   Serial.println("no conectado");
  }
  
  if(Estado1 == 1){ 
  for(int i=0; i<1; i=i +1){ //Se enviarán 1 mensajes 
  Serial.println("embiado");
  
  smtp.debug(1);

 

  /* Declare the session config data */
  ESP_Mail_Session session;


  /* Configura la sesión */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declara la clase del mensaje */
  SMTP_Message message;

  /* Configura cabecera del mensaje */
  message.sender.name = "Riego";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Alerta";
  message.addRecipient("BY:JCNF", RECIPIENT_EMAIL);
 
  //Manda texto
  String textMsg = "Se ha encendido el sistema de riego exitosamente";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  /* Conecta al servidor */
  if (!smtp.connect(&session))
    return;

  /* Manda correo y cierra sesiÃ³n */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
 Estado1 =0;
}

}
delay(200);
}