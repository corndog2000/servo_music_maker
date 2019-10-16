//Current servo-note assignments
//C, D, E, F, G

Servo * findServo(String input)
{
  for (int i = 0; i < NUM_SERVOS; ++i)
  {

    if (input.substring(0, input.length() - 1).equals(notes[i]))
    {
      Serial.print("DEBUG ");
      Serial.println(input.substring(0, input.length() - 1));

      Serial.print("found servo: ");
      Serial.println(i);
      return &servos[i];
    }
  }
}

void initialize_servos()
{
  for (int i = 0; i < NUM_SERVOS; i++)
  {
    if (pins[i] != -1)
    {
      servos[i].write(0);
    }
  }
}

//New method based on servo array
void servo_on(String input)
{
  Serial.print("input ");
  Serial.println(input);

  findServo(input)->write(20);
}

void servo_off(String input)
{
  findServo(input)->write(0);
}


//Old method of controlling servo motors
/*
  void servo_on(String input)
  {
  Serial.print("input ");
  Serial.println(input);
  if (input.indexOf("C#") != -1)
  {
    s23.write(20);
  }
  else if (input.indexOf("C") != -1)
  {
    s22.write(20);
  }
  else if (input.indexOf("D") != -1)
  {
    s24.write(20);
  }
  else if (input.indexOf("Eb") != -1)
  {
    s25.write(20);
  }
  else if (input.indexOf("E") != -1)
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
*/
