mailsend.exe -to %1 -from %2 -sub "test from windows" -starttls -port 587 -auth -smtp smtp.gmail.com -user "%2" -pass %SMTP_USER_PASS% -separator "|" -attach "c:\File,with,comma.txt|text/plain" -v
