//TESTE DE CALIBRAÇÃO


// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>
#include <EEPROM.h>

// Endereço na EEPROM para armazenar o float
#define EEPROM_ADDR 0

// DEFINIÇÕES DE PINOS
#define pinDT  2
#define pinSCK  3

HX711 balanca;

#define scaleArmazenado 0
#define offsetArmazenado 0
float valorScale = 0;
float valorOffset = 0;





void setup() {
  Serial.begin(115200);
  Serial.println("Laica - Balanceitor");
  balanca.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA

  // EEPROM.get(scaleArmazenado, valorScale);111

  // balanca.set_scale(valorScale);
  // balanca.set_offset(valorOffset);

  // Serial.print (valorScale);
  // Serial.print ("       ");
  // Serial.println (valorOffset);
  delay(1000);

  //scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA


  Serial.println("Pressione 0 para ajuda");

}

void loop(){
  if (Serial.available() == 0) {
    Serial.println(medir(),3);
  } else {
    char c = Serial.read();
    switch (c){
      case '0':
        Ajuda();
        break;
      case '1':
        Serial.println(" Tara Pressionado: ");
        balanca.tare();
        break;
      case '2':
        Serial.println(" Offset Pressionado: ");
        Serial.println(balanca.get_offset());
        break;
      case '3':
        calibrate();
        break;
      default:
        Serial.println("Segue o jogo");
        break;
    }
  }
}

void Ajuda(){
  Serial.println("Menu: \n\t0 para ajuda\n\t1 para tara\n\t2 valor offset\n\t3 - Calibrar");
}

float medir() {
  return balanca.get_units(0);
}

void calibrate()
{
  Serial.println("\n\nCALIBRATION\n===========");
  Serial.println("remove all weight from the loadcell");
  //  flush Serial input
  while (Serial.available()) Serial.read();

  Serial.println("and press enter\n");
  while (Serial.available() == 0);

  Serial.println("Determine zero weight offset");
  //  average 20 measurements.
  balanca.tare(20);
  int32_t offset = balanca.get_offset();

  Serial.print("OFFSET: ");
  
  Serial.println(offset);
  Serial.println();


  Serial.println("place a weight on the loadcell");
  //  flush Serial input
  while (Serial.available()) Serial.read();

  Serial.println("enter the weight in (whole) grams and press enter");
  uint32_t weight = 5000;
  

 while (Serial.available() == 0);

  Serial.print("WEIGHT: ");
  Serial.println(weight);
  balanca.calibrate_scale(weight, 20);
  float scale = balanca.get_scale();
  balanca.set_scale(scale);
  balanca.set_offset(offset);
  Serial.print("SCALE:  ");
  Serial.println(scale, 6);

  Serial.print("\nuse scale.set_offset(");
  Serial.print(offset);
  Serial.print("); and scale.set_scale(");
  Serial.print(scale, 6);
  Serial.print(");\n");
  Serial.println("in the setup of your project");

  Serial.println("\n\n");

  valorScale = scale;
  valorOffset = offset;

  // EEPROM.put(scaleArmazenado, valorScale);
  // EEPROM.put(offsetArmazenado, valorOffset);

  Serial.print (scaleArmazenado);
  Serial.print ("/t");
  Serial.println (offsetArmazenado);
  
  delay(1000);

}
