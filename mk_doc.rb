#!/usr/bin/env ruby

# make the minimal pod doc and then generate man page from it
# muquit@muquit.com  Mar-27-2012 
class MakeMailsendPod

  PROG = "./mailsend"

  def initialize
    @mailsend_ver = ''
    @copyright
  end

  def log(msg)
    t = Time.new
    puts "#{t}: #{msg}"
  end

  def mailsend_version
    ver = `./mailsend -V 2>&1`
    a = ver.split("\n")
    l = a[0].to_s.chomp
    if l =~ /.*v(.*)$/
      @mailsend_ver = $1
      log "mailsend version: '#{@mailsend_ver}'"
    else
      log "Could not determine mailsend version"
      exit(1)
    end
  end

  def check_binary
    if !File.exists?("#{PROG}")
      log "#{PROG} does not exist. exiting"
      exit
    end
  end

  def gen_pod
    synopsis = `#{PROG} -h`
    ex = `#{PROG} -ex`
    f = File.open("./doc/mailsend.pod", "w")
    f.puts <<EOD
=head1 NAME

mailsend - A command line program to send mail via SMTP protocol

=head1 SYNOPSYS

#{synopsis}

=head1 DESCRIPTION

mailsend is a simple command line program to send mail via SMTP protocol 
for Windows, Linux/Unix.

For deatils, please look at the web page:

L<http://muquit.com/muquit/software/mailsend/mailsend.html>

=head1 EXAMPLES

#{ex}

=head1 SEE ALSO

Please look at the web page for latest version and documentation:
L<http://muquit.com/muquit/software/mailsend/mailsend.html>


=head1 LICENSE

#{@copyright}

=head1 AUTHOR

mailsend is written by Muhammad Muquit E<lt>muquit@muquit.comE<gt>.
Homepage: L<http://www.muquit.com/>.

EOD
;
  f.close
  end

  def read_copyright
    @copyright = File.read("./COPYRIGHT")
  end

  def gen_man
    cmd = "pod2man --release \"mailsend #{@mailsend_ver}\" -c \"User Commands\" doc/mailsend.pod > doc/mailsend.1"
    system(cmd)
    # compress
    cmd = "gzip --best -c doc/mailsend.1 > doc/mailsend.1.gz"
    system(cmd)
  end

  def doit
    check_binary
    mailsend_version
    read_copyright
    gen_pod
    gen_man
  end
end

if __FILE__ == $0
  MakeMailsendPod.new.doit
end
