
void onMqttMessage(char* topic, char* payload, const AsyncMqttClientMessageProperties& properties,
                   const size_t& len, const size_t& index, const size_t& total) {
  (void)payload;

  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  String messageTemp;
  for (uint8_t i = 0; i < len; i++) {
    messageTemp += (char)payload[i];
  }
  valNaliv = messageTemp.toInt();
  Serial.println(messageTemp);
  if (strcmp(topic, Nalivator) == 0) {
   Voice_command ();
  }
}
