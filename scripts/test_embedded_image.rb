#!/usr/bin/env ruby
# Muhammad.Muquit@bt.com Dec-24-2013 
class TestEmbeddedImage
  PROG = Dir.pwd + "/mailsend"
  ME = File.basename($0)
  def initialize
    $stdout.sync = true
    if !File.exists?(PROG)
      log "Error: #{PROG} does not exit"
      exit 0
    end
    @pass = nil
    @to = nil
    @from = nil
  end

  def log(msg)
    puts "#{msg}"
  end


  def check_args
    if ARGV.length != 2
      puts <<-EOF
  Usage: #{ME} <to@gmail.com> <from@gmail.com>"
EOF
      exit 1
    end
    @to = ARGV[0]
    @from = ARGV[1]
  end

  def check_pass
    @pass = ENV['SMTP_USER_PASS']
    if !@pass
      puts <<-EOF
  Error: SMTP_USR_PASS env var is not set
EOF
      exit
    end
  end

  def send_mail
    html_file = Dir.pwd + "/test/embed_image1.html"
    image = Dir.pwd + "/test/test.png"
    content_id = "xx_deadbeefcafe"

    cmd = ""
    cmd << PROG
    cmd << " "
    cmd << "-v"
    cmd << " "
    cmd << "-sub"
    cmd << " "
    cmd << "\"Testing embedding image in HTML\""
    cmd << " "
    cmd << "-from"
    cmd << " "
    cmd << @from
    cmd << " "
    cmd << "-to"
    cmd << " "
    cmd << @to
    cmd << " "
    cmd << "-smtp smtp.gmail.com -port 587 -starttls -auth"
    cmd << " "
    cmd << "-user"
    cmd << " "
    cmd << @from
    cmd << " "
    cmd << "-cs"
    cmd << " "
    cmd << "ISO-8859-1"
    cmd << " "
    cmd << "-content-type \"multipart/related\""
    cmd << " "
    cmd << "-mime-type text/html"
    cmd << " "
    cmd << "-disposition inline"
    cmd << " "
    cmd << "-enc-type \"none\""
    cmd << " "
    cmd << "-attach \"#{html_file}\""
    cmd << " "
    cmd << "-mime-type image/png"
    cmd << " "
    cmd << "-enc-type \"base64\""
    cmd << " "
    cmd << "-disposition inline"
    cmd << " "
    cmd << "-content-id \"#{content_id}\""
    cmd << " "
    cmd << "-cs \"none\""
    cmd << " "
    cmd << "-attach \"#{image}\""

    log "run: #{cmd}"
#    system(cmd)
  end

  def doit
    check_args
    check_pass
    send_mail
  end
end

if __FILE__ == $0
  TestEmbeddedImage.new.doit
end
