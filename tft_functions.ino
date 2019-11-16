void clearScreen() {
  tft.fillScreen(ILI9341_BLACK);
  if (verboseMode) {
    Serial.println("Cleared Screen");
  }
}

void drawSelectionButtons(bool sdFail) {
  if (!sdFail)
  {
    SelectSD.drawButton();
    NextFile.drawButton();
    //drawFilename(tuneList[0]);
    if (verboseMode) {
      Serial.println("Drew SelectSD Button");
    }
  }

  SelectMidi.drawButton();
  if (verboseMode) {
    Serial.println("Drew SelectMidi Button");
  }
}

void drawFilename(String labelText) {
  tft.fillRect(10, 135, 320, 20, ILI9341_BLACK);
  
  tft.setCursor(10, 135);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  Serial.print(F("Drawing: "));
  Serial.println(labelText);
  tft.println(labelText);
}

void drawSelectInputLabel() {
  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(4);
  tft.println("Select Input");
}

void drawPlayingSdCardLabel() {
  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(4);
  tft.println("Playing SD");
}

void drawHomeScreen(bool sdFail)
{
  if (sdFail)
  {
    //tft.fillScreen(ILI9341_ORANGE);
    tft.fillScreen(ILI9341_BLACK);
    
  }
  else
  {
    tft.fillScreen(ILI9341_BLACK);
  }
  
  drawFilename("Hello World");
  drawSelectInputLabel();
  drawSelectionButtons(sdFail);
}

void drawPlayingSdCard()
{
  tft.fillScreen(ILI9341_BLACK);
  drawPlayingSdCardLabel();
}
