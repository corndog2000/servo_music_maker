//Current servo-note assignments
//C, D, E, F, G

void initialize_servos()
{
  for (int i = 0; i < sizeof(servos); i++)
  {
    servos[i].attach(servo_pins[i]);
  }

  /*
  s22.attach(22); //C
  s23.attach(23); //D
  s24.attach(24); //E
  s25.attach(25); //F
  s26.attach(26); //G
  */
}

void servo_on(String input)
{
  if (strchr(input, 'C') != NULL)
  {
    s22.write(20);
  }
  if (strchr(input, 'D') != NULL)
  {
    s23.write(20);
  }
  if (strchr(input, 'E') != NULL)
  {
    s24.write(20);
  }
  if (strchr(input, 'F') != NULL)
  {
    s25.write(20);
  }
  if (strchr(input, 'G') != NULL)
  {
    s26.write(20);
  }
}

void servo_off(String input)
{
  if (strchr(input, 'C') != NULL)
  {
    s22.write(0);
  }
  if (strchr(input, 'D') != NULL)
  {
    s23.write(0);
  }
  if (strchr(input, 'E') != NULL)
  {
    s24.write(0);
  }
  if (strchr(input, 'F') != NULL)
  {
    s25.write(0);
  }
  if (strchr(input, 'G') != NULL)
  {
    s26.write(0);
  }
}

void servo_tap(String input)
{
  //myservo.write(0);
  //delay(300);
  myservo.write(20);
  delay(100);
  myservo.write(0);
}
