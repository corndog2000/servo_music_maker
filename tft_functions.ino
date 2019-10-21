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
    if (verboseMode) {
      Serial.println("Drew SelectSD Button");
    }
  }

  SelectMidi.drawButton();
  if (verboseMode) {
    Serial.println("Drew SelectMidi Button");
  }
  /*
    Number2.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 2 Button");
    }
    Number3.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 3 Button");
    }
    Number4.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 4 Button");
    }
    Number5.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 5 Button");
    }
    Number6.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 6 Button");
    }
    Number7.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 7 Button");
    }
    Number8.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 8 Button");
    }
    Number9.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number 9 Button");
    }
    NumberDelete.drawButton();
    if (verboseMode) {
    Serial.println("Drew Number Delete Button");
    }
  */
}

void drawSelectInputLabel() {
  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(4);
  tft.print("Select Input");
}

void drawPlayingSdCardLabel() {
  tft.setCursor(10, 10);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(4);
  tft.print("Playing SD");
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
  drawSelectInputLabel();
  drawSelectionButtons(sdFail);
}

void drawPlayingSdCard()
{
  tft.fillScreen(ILI9341_BLACK);
  drawPlayingSdCardLabel();
}
