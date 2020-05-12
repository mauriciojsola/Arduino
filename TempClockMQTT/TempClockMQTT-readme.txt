https://learn.adafruit.com/home-automation-in-the-cloud-with-the-esp8266-and-adafruit-io/hardware-and-software-requirements

https://github.com/openhomeautomation/adafruit-io-esp8266/blob/master/esp8266_sensor_module/esp8266_sensor_module.ino
https://www.electronics-lab.com/project/getting-started-with-adafruit-io-iot-cloud-platform/


https://io.adafruit.com/api/v2/mauriciojsola11/feeds/weather-feed/data.json?X-AIO-Key=aio_fZrg70SzjQj4UYoJfxJcBKmQQdVE


{
  "value": "HELLO",
  "lat": 38.1123,
  "lon": -91.2325,
  "ele": 112
}


IMPORTANTE!!!: Content-Type: application/json


https://github.com/bblanchon/ArduinoJson
https://arduinojson.org/book/serialization_tutorial6.pdf
https://arduinojson.org/v6/doc/serialization/
https://arduinojson.org/v6/assistant/
https://github.com/bblanchon/ArduinoJson/issues/869   // Serialize to variable!
https://www.mischianti.org/2020/01/26/manage-json-file-with-arduino-and-esp8266/

https://io.adafruit.com/api/v2/mauriciojsola11/feeds/weather-feed.wheater-checkpoint/data.json?X-AIO-Key=aio_fZrg70SzjQj4UYoJfxJcBKmQQdVE
{
  "value": "{\"timestamp\":\"2020-05-11 15:44\", \"humidity\": \"60.40\", \"temperature\":\"21.5\"}",
  "lat": 0,
  "lon": 0,
  "ele": 0
}


---------
connected with Fibertel WiFi587 2.4GHz, channel 1
dhcp client start...
Date/Time: ip:192.168.0.196,mask:255.255.255.0,gw:192.168.0.1


https://forum.arduino.cc/index.php?topic=515400.15
https://ricveal.com/blog/arduino-enviando-datos-parte-3/   rest client -> https://github.com/fabianofranca/ESP8266RestClient
https://forum.arduino.cc/index.php?topic=515400.15   WITH DATA

{"value":{"timestamp":"2020-05-11 22:50:59","humidity":43.9,"temperature":22.7},"lat":0}

--------------

[Response:]
HTTP/1.1 200 OK

Server: nginx

Date: Tue, 12 May 2020 20:52:50 GMT

Content-Type: application/json; charset=utf-8

Transfer-Encoding: chunked

Connection: close

X-Frame-Options: SAMEORIGIN

X-XSS-Protection: 1; mode=block

X-Content-Type-Options: nosniff

X-Download-Options: noopen

X-Permitted-Cross-Domain-Policies: none

Referrer-Policy: strict-origin-when-cross-origin

X-AIO-Worker: io-rails-1

Access-Control-Allow-Origin: *

Access-Control-Allow-Credentials: false

Access-Control-Request-Method: *

Access-Control-Allow-Methods: POST, PUT, DELETE, GET, OPTIONS, PATCH

Access-Control-Allow-Headers: DNT, Origin, X-Requested-With, X-AIO-Key, Content-Type, Accept, Authorization

Access-Control-Expose-Headers: X-Pagination-Limit, X-Pagination-Start, X-Pagination-End, X-Pagination-Count, X-Pagination-Total

Access-Control-Max-Age: 1728000

ETag: W/"4c4c9b6ad2376471855d5252cf1e43e8"

Cache-Control: max-age=0, private, must-revalidate

X-Request-Id: b240416a-99d3-4f2f-a375-e42871a34110

X-Runtime: 0.072698

X-AIO-Proxy: 2



194

{"id":"0EEHN9V4J4NSARJB52AH9Z7ZDG","value":"{\"timestamp\":\"2020-05-12 17:52:27\",\"humidity\":45.80,\"temperature\":23.80}","feed_id":1373814,"feed_key":"weather-feed.wheater-checkpoint","created_at":"2020-05-12T20:52:50Z","location":{"type":"Feature","geometry":{"type":"Point","coordinates":[0.0,0.0,0.0]}},"lat":0.0,"lon":0.0,"ele":0.0,"created_epoch":1589316770,"expiration":"2020-06-11T20:52:50Z"}

0

