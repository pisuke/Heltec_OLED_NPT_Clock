//#include <WiFi.h>

DynamicJsonDocument httpJSONRequest(WiFiClient client, char *hostname, String endpoint) {
  const size_t capacity = JSON_OBJECT_SIZE(15) + 350;
  DynamicJsonDocument doc(capacity);
  
  // Connect to HTTP server
  client.setTimeout(10000);
  if (!client.connect(hostname, 80)) {
    Serial.println(F("Connection failed"));
    return doc;
  }

  Serial.println(F("HTTP request client connected"));

  // Send HTTP request
  client.print(F("GET /"));
  client.print(endpoint);
  client.println(F(" HTTP/1.0"));
  client.print(F("Host: "));
  client.println(hostname);
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return doc;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return doc;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return doc;
  }

  // Parse the JSON response
  deserializeJson(doc, client);
  return doc;
}
