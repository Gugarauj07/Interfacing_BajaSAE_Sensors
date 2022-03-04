void temperatura()
{
  if((millis() - Temp_millisInicial) > 1000){
    //Leitura da temperatura ambiente e do objeto
    //(para leitura dos valores em Fahrenheit, utilize
    //mlx.readAmbientTempF() e mlx.readObjectTempF()
  
    temp_amb = mlx.readAmbientTempC();
    temp_obj = mlx.readObjectTempC();
    Serial.print("Ambiente = ");
    Serial.print(temp_amb);
    Serial.print("   Objeto = ");
    Serial.println(temp_obj); 
    Serial.println("===================================");
    Temp_millisInicial = millis();
  }
}
