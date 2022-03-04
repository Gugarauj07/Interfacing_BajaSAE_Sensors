void hc12() {

  if((millis()-telemetria_millisInicial) > 1000){
    noInterrupts();
    armazenar = (String(temp_obj) + "," + String(VEL) + "," + String("RPM"));
    HC12.println(armazenar);
    telemetria_millisInicial = millis();
    interrupts();
  }
}
