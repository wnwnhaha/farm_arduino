// 发送温度
void sendTemp() {
  f_head head;
  f_05 f05;
  int16_t crc;

  int b_length = sizeof(f_05); // 消息体长度

  int16_t d_length = b_length + sizeof(f_head) + sizeof(int16_t);  // 数据包总长度
  char * data =  (char *)malloc(d_length);
  int d_position = 0; //指针赋值标签

  // 消息体头部
  head.length = b_length;
  head.sequence = sequence++;
  head.type = 5;
  memcpy(data + d_position, &head, sizeof(f_head));
  d_position += sizeof(f_head);

  // 消息体内容
  float temp = sht1x.readTemperatureC();
  f05.temperature1 = (int8_t)temp;
  f05.temperature0 = (uint8_t)(temp * 100) % 100;
  memcpy(data + d_position, &f05, b_length);
  d_position += b_length;

  // 消息体的校验
  uint8_t tobyte[b_length];
  memcpy(tobyte, &f05, b_length);
  crc = crc16(tobyte, b_length);
  memcpy(data + d_position, &crc, b_length);

  // 发送
  uint8_t d_tobyte[d_length];
  memcpy(d_tobyte, data, d_length);
  Serial.write(d_tobyte, d_length);
}

