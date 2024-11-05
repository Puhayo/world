function doPost(e) {
    try {
      const spreadsheetId = '1eA4w_tnDekfNnifwhez-HRdCOqJmXErf609nrmpvAAM';
      const ss = SpreadsheetApp.openById(spreadsheetId);
      
      // 데이터 파싱 시도
      let studentId, password;
      
      if (e && e.postData) {
        // POST 데이터로 시도
        const postData = JSON.parse(e.postData.contents);
        studentId = postData.studentId;
        password = postData.password;
      } else if (e && e.parameter) {
        // URL 파라미터로 시도
        studentId = e.parameter.studentId;
        password = e.parameter.password;
      } else {
        // 테스트용 기본값
        studentId = "20230001";
        password = "홍길동";
      }
      
      Logger.log("받은 데이터 - 학번: " + studentId + ", 이름: " + password);
      
      // 메인 시트에서 사용자 확인
      const mainSheet = ss.getSheetByName('시트1');
      const data = mainSheet.getDataRange().getValues();
      
      // 사용자 확인
      let found = false;
      for (let row of data) {
        if (row[0].toString() === studentId && row[1].toString() === password) {
          found = true;
          break;
        }
      }
      
      if (found) {
        // 로그 시트에 기록
        let logSheet = ss.getSheetByName('로그');
        if (!logSheet) {
          logSheet = ss.insertSheet('로그');
          logSheet.appendRow(['학번', '이름', '로그인 시간']);
        }
        
        // 한국 시간으로 변환
        const timestamp = new Date();
        const koreanTime = Utilities.formatDate(timestamp, "Asia/Seoul", "yyyy-MM-dd HH:mm:ss");
        
        // 로그 추가
        logSheet.appendRow([studentId, password, koreanTime]);
        Logger.log("로그 기록 완료");
        
        return ContentService.createTextOutput(JSON.stringify({ authorized: true }))
          .setMimeType(ContentService.MimeType.JSON);
      }
      
      return ContentService.createTextOutput(JSON.stringify({ authorized: false }))
        .setMimeType(ContentService.MimeType.JSON);
      
    } catch (error) {
      Logger.log("오류: " + error.toString());
      return ContentService.createTextOutput(JSON.stringify({
        authorized: false,
        error: error.toString()
      })).setMimeType(ContentService.MimeType.JSON);
    }
  }