/*******************************************************
 * Intellectual Property Notice
 *
 * All the contents of this software/code, including but not limited to
 * algorithms, designs, methodologies, and ideas (collectively referred
 * to as "the Contents"), are the intellectual property of Digital Sports Solutions
 * LLC. The Contents are protected by intellectual property laws and all rights
 * are reserved.
 *
 * Unauthorized use, reproduction, or distribution of the Contents,
 * in whole or in part, is strictly prohibited and may result in legal
 * action.
 *
 * For inquiries or licensing opportunities, please contact
 * adamflis2002@gmail.com
 *******************************************************/

#include "main.hpp"

// Constants
const String UNDEFINED_STR = "undefined";

// Update min/max function that gets called from server
void updateIndex(const String& min, const String& max) {
    if (min != UNDEFINED_STR && max != UNDEFINED_STR) {
        setMin(min.toInt());
        setMax(max.toInt());
        team1.updateResetMax();
        team2.updateResetMax();        
    }
    updateClient();
}

// Convert RGB to hex
String rgbToHex(uint8_t r, uint8_t g, uint8_t b) {
  String red = String(r, HEX);
  String green = String(g, HEX);
  String blue = String(b, HEX);

  String hex = "#";
  if (red.length() == 1) {
    hex += "0";
  }
  hex += red;
  if (green.length() == 1) {
    hex += "0";
  }
  hex += green;
  if (blue.length() == 1) {
    hex += "0";
  }
  hex += blue;
  
  return hex;
}

// Get data for event triggers/updates
String getData() {
    // Create JSON object
    DynamicJsonDocument doc(2048);

    // Create the JSON catagories
    JsonObject team_1 = doc.createNestedObject("team1");
    JsonObject team_2 = doc.createNestedObject("team2");
    JsonObject gameClock = doc.createNestedObject("gameClock");
    JsonObject other = doc.createNestedObject("other");

    // Team 1
    team_1["trueCount"] = team1.getCount();
    team_1["displayedCount"] = team1.convertNumber(team1.getCount());
    team_1["playersLeft"] = team1.getPlayers();
    team_1["timeoutsLeft"] = team1.getTimeouts();
    team_1["points"] = team1.getPoints();
    team_1["color"]["hex"] = rgbToHex(team1.getRed(), team1.getGreen(), team1.getBlue());
    team_1["color"]["rgb"]["r"] = team1.getRed();
    team_1["color"]["rgb"]["g"] = team1.getGreen();
    team_1["color"]["rgb"]["b"] = team1.getBlue();
    team_1["paused"] = team1.getPaused();
    team_1["countDirection"] = team1.getDirection();
    team_1["countDuration"] = team1.getDuration();
    team_1["violation"] = team1.getViolation();
    team_1["side"] = team1.getSide();
    team_1["name"] = team1.getName();
    
    // Team 2
    team_2["trueCount"] = team2.getCount();
    team_2["displayedCount"] = team2.convertNumber(team2.getCount());
    team_2["playersLeft"] = team2.getPlayers();
    team_2["timeoutsLeft"] = team2.getTimeouts();
    team_2["points"] = team2.getPoints();
    team_2["color"]["hex"] = rgbToHex(team2.getRed(), team2.getGreen(), team2.getBlue());
    team_2["color"]["rgb"]["r"] = team2.getRed();
    team_2["color"]["rgb"]["g"] = team2.getGreen();
    team_2["color"]["rgb"]["b"] = team2.getBlue();
    team_2["paused"] = team2.getPaused();
    team_2["countDirection"] = team2.getDirection();
    team_2["countDuration"] = team2.getDuration();
    team_2["violation"] = team2.getViolation();
    team_2["side"] = team2.getSide();
    team_2["name"] = team2.getName();

    // Game Clock
    gameClock["displayedMinutes"] = gameClk.getDisplayMin();
    gameClock["displayedSeconds"] = gameClk.getDisplaySec();
    gameClock["minutes"] = gameClk.getMin();
    gameClock["seconds"] = gameClk.getSec();
    gameClock["timeout"] = gameClk.getTimeout();
    gameClock["timeoutMinutes"] = gameClk.getTimeoutMin();
    gameClock["timeoutSeconds"] = gameClk.getTimeoutSec();
    gameClock["half"] = gameClk.getHalf();
    gameClock["name"] = gameClk.getName();
    gameClock["tournamentName"] = gameClk.getTournamentName();
    gameClock["middleOfPoint"] = gameClk.getMidPoint();
    gameClock["paused"] = gameClk.getPaused();
    gameClock["color"]["hex"] = rgbToHex(gameClk.getRed(), gameClk.getGreen(), gameClk.getBlue());
    gameClock["color"]["rgb"]["r"] = gameClk.getRed();
    gameClock["color"]["rgb"]["g"] = gameClk.getGreen();
    gameClock["color"]["rgb"]["b"] = gameClk.getBlue();

    // Other
    other["minimumLength"] = getMin();
    other["maximumLength"] = getMax();  

    // Send JSON object to client
    String ret;
    serializeJson(doc, ret);
    return ret;
}
void updateClient() {
    String data = getData();
    ws.textAll(data);
    writeData("data.txt", data);
}