/**
 * Google Apps Script — Activity Logger
 * =====================================
 * Deploy as a Web App:
 * Extensions → Apps Script → paste this → Deploy → New Deployment
 * Type: Web App | Execute as: Me | Who has access: Anyone
 *
 * Your sheet must have columns:  Timestamp | Action | Value
 *
 * The ESP32 calls via HTTP POST:
 * JSON Payload: {"Action":"Pull Ups", "Value":"1"}
 */
function doPost(e) {
  try {
    // 1. Get the JSON data sent from the ESP32
    var jsonString = e.postData.contents;
    var data = JSON.parse(jsonString);

    // 2. Connect to the active sheet
    var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();

    // 3. Create a timestamp for right now
    var timestamp = new Date();

    // 4. Extract the Action and Value from the ESP32 payload
    var action = data.Action;
    var value = data.Value;

    // 5. Append a new row with the data
    sheet.appendRow([timestamp, action, value]);

    // 6. Send a "Success" message back to the ESP32 (HTTP 200)
    return ContentService.createTextOutput("Success")
                         .setMimeType(ContentService.MimeType.TEXT);

  } catch (error) {
    // If something goes wrong, send the error back
    return ContentService.createTextOutput("Error: " + error.toString())
                         .setMimeType(ContentService.MimeType.TEXT);
  }
}
