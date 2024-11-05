function doPost(e) {
    const sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
    let data;
    
    try {
      // POST 데이터가 문자열로 전송된 경우
      data = JSON.parse(e.postData.getDataAsString());
    } catch (error) {
      // 오류 발생 시 기본값 설정
      data = {
        studentId: 'unknown',
        eventType: 'error',
        eventDetail: error.toString(),
        sessionTime: 0
      };
    }
    
    try {
      sheet.appendRow([
        new Date(),
        data.studentId,
        data.eventType,
        data.eventDetail,
        data.sessionTime
      ]);
      
      return ContentService.createTextOutput(JSON.stringify({
        'status': 'success'
      })).setMimeType(ContentService.MimeType.JSON);
    } catch (error) {
      return ContentService.createTextOutput(JSON.stringify({
        'status': 'error',
        'message': error.toString()
      })).setMimeType(ContentService.MimeType.JSON);
    }
  }