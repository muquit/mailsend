#!/usr/bin/env ruby

# muquit@muquit.com May-05-2013 
# a helper script to upload files
begin
  ME = $0
  if ARGV.length != 2
    puts <<EOD
#{ME} "<comment>" "<file">
EOD
    exit
  end
  if !ENV['GOOGLECODE_PASS']
    puts "Set GOOGLECODE_PASS"
    exit 1
  end
  comment = ARGV[0]
  file = ARGV[1]
  cmd = "googlecode_upload.pl"
  cmd << " "
  cmd << "-s="
  cmd << '"'
  cmd << comment
  cmd << '"'
  cmd << " "
  cmd << "-project="
  cmd << '"mailsend"'
  cmd << " "
  cmd << '-user="muquit"'
  cmd << " "
  cmd << '-labels="Featured"'
  cmd << " "
  cmd << "--file="
  cmd << file
  puts cmd
 system(cmd)

rescue => e
  puts "Error: #{e}"
end
