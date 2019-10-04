//Current servo-note assignments
//C, D, E, F, G

void initialize_servos()
{
  s22.write(0);
  s23.write(0);
  s24.write(0);
  s25.write(0);
  s26.write(0);
}

void servo_on(String input)
{
  Serial.print("input ");
  Serial.println(input);
  if (input.indexOf("C") != -1)
  {
    s22.write(20);
  }
  else if (input.indexOf("D") != -1)
  {
    s23.write(20);
  }
  else if (input.indexOf("E") != -1)
  {
    s24.write(20);
  }
  else if (input.indexOf("F") != -1)
  {
    s25.write(20);
  }
  else if (input.indexOf("G") != -1)
  {
    s26.write(20);
  }
}

void servo_off(String input)
{
  if (input.indexOf("C") != -1)
  {
    s22.write(0);
  }
  else if (input.indexOf("D") != -1)
  {
    s23.write(0);
  }
  else if (input.indexOf("E") != -1)
  {
    s24.write(0);
  }
  else if (input.indexOf("F") != -1)
  {
    s25.write(0);
  }
  else if (input.indexOf("G") != -1)
  {
    s26.write(0);
  }
}


void servo_tap(String input)
{
  Serial.print("input ");
  Serial.println(input);
  if (input.indexOf("C") != -1)
  {
    s22.write(20);
    delay(100);
    s22.write(0);
  }
  else if (input.indexOf("D") != -1)
  {
    s23.write(20);
    delay(100);
    s23.write(0);
  }
  else if (input.indexOf("E") != -1)
  {
    s24.write(20);
    delay(100);
    s24.write(0);
  }
  else if (input.indexOf("F") != -1)
  {
    s25.write(20);
    delay(100);
    s25.write(0);
  }
  else if (input.indexOf("G") != -1)
  {
    s26.write(20);
    delay(100);
    s26.write(0);
  }
}
