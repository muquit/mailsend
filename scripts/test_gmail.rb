#!/usr/bin/env ruby


# Test sending mail via gmail
# muquit@muquit.com Apr-20-2012 
class TestGmail
  ME = $0

  def initialize
    @to = ''
    @from = ''
    @password = ''
    @options = "-v "
    @subject = 'Testing mailsend'
    @smtp_server = "smtp.gmail.com"
    @starttls_port = "587"
    @ssl_port = "465"
  end

  def log(msg)
    t = Time.new
    puts "#{t}: #{msg}"
  end

  def check_args
    if ARGV.length < 2
      puts <<EOD
  #{ME} <from_address> <to_address> [password option]
  Example:
    #{ME} gmail_user foo@example.com '-pass <plaintext password>'
EOD
# The feature is removed
#    #{ME} gmail_user foo@example.com '-encpass <encrypted password>'
      exit 1
    end

    @from = ARGV[0]
    @to = ARGV[1]

    if ARGV.length == 3
      x = ARGV[2]
      case x
      when /^-pass (.*)$/
        p = $1
        if RUBY_PLATFORM =~ /(win|w)32$/
          @password = "-pass \"#{p}\" "
        else
          @password = "-pass '#{p}' "
        end
      when /^-encpass (.*)$/
        @password = "-encpass '#{$1}' "
      else
        puts "Invalid flags"
        exit 1
      end
    end
  end

  def add_starttls_options(cmd)
    cmd.concat("-sub ")
    t = Time.new
    cmd.concat('"')
    cmd.concat(@subject)
    cmd.concat(' ')
    cmd.concat(t.to_s)
    cmd.concat(' starttsl"')
    cmd.concat(' ')
    cmd.concat("-starttls ")
    cmd.concat("-port #{@starttls_port} ")
    return cmd
  end

  def add_ssl_options(cmd)
    cmd.concat("-sub ")
    t = Time.new
    cmd.concat('"')
    cmd.concat(@subject)
    cmd.concat(' ')
    cmd.concat(t.to_s)
    cmd.concat(' ssl"')
    cmd.concat(' ')
    cmd.concat("-ssl ")
    cmd.concat("-port #{@ssl_port} ")
    return cmd
  end

  def make_command(type)
    t = Time.new
    cmd = "./mailsend "
    if RUBY_PLATFORM =~ /(win|w)32$/
      cmd = "mailsend.exe "
    end
    cmd.concat(@options)

    cmd.concat("-smtp #{@smtp_server} ")

    if (type == "starttls")
      cmd = add_starttls_options(cmd)
    elsif (type == "ssl")
      cmd = add_ssl_options(cmd)
    end

    cmd.concat(" -user #{@from}")
    cmd.concat(" -auth ")
    cmd.concat(@password)
    cmd.concat(' ')

    cmd.concat("-f #{@from}.gmail.com ")
    cmd.concat("-t #{@to} ")

    log(cmd)
    return cmd
  end

  def send_mail
    if !File.exists?("./mailsend")
      puts "Compile mailsend first"
      exit 1
    end
    cmd = make_command("starttls")
    system(cmd)
    cmd = make_command("ssl")
    system(cmd)
  end
  
  def send_mail_attachment1
    cmd = make_command("starttls")
    cmd.concat("-attach ")
    cmd.concat('"')
    cmd.concat("test.txt,text/plain");
    cmd.concat('" ')
    log(cmd)
    system(cmd)

    cmd = make_command("starttls")
    cmd.concat("-attach ")
    cmd.concat('"')
    cmd.concat("test.txt,text/plain,i");
    cmd.concat('" ')
    log(cmd)
    system(cmd)

  end

  def test
    check_args
#    send_mail
    send_mail_attachment1
  end
end

if __FILE__ == $0
  gmail = TestGmail.new
  gmail.test
end

