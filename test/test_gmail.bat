mailsend.exe -to %1 -from %2 -sub "test from windows" -starttls -port 587 -auth -smtp smtp.gmail.com -user "%2" -pass %SMTP_USER_PASS% -M "hello one liner" -log "c:\Users\mqt\Downloads\m.log"
