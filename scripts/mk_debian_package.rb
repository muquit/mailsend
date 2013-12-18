#!/usr/bin/env ruby

##################################################
# make a debian binary package for ubuntu
# muquit@muquit.com Mar-28-2012 
##################################################
#
require 'fileutils'

class MakeDebianPackage

  def initialize
    @dirs = []

    @dirs << Dir.pwd + "/debian/DEBIAN"
    @dirs << Dir.pwd + "/debian/usr/bin"
    @dirs << Dir.pwd + "/debian/usr/share/doc/mailsend"
    @dirs << Dir.pwd + "/debian/usr/share/man/man1"

    @depends = "libc6 (>= 2.4)" # XXX CHANGE if needed
    @mailsend_ver = ''
    @package_name = ''
    @control_file = Dir.pwd + "/debian/DEBIAN/control"
  end

  def log(msg)
    t = Time.new
    puts "#{t}: #{msg}"
  end

  def check
    file = Dir.pwd + "/mailsend"
    if !File.exists?(file)
      log "#{file} does not exist, compile first"
      exit(1)
    end

    file = Dir.pwd + "/changelog.debian"
    if !File.exists?(file)
      log "#{file} does not exist, compile first"
      exit(1)
    end

  end

  def gen_control_file
    mailsend_version
    begin
    f = File.open(@control_file, "w")
    f.puts <<EOD
Package: mailsend
Section: utils
Version: #{@mailsend_ver}-ubuntu
Maintainer: Muhammad Muquit <muquit@muquit.com>
Installed-Size: 104
Homepage: http://muquit.com/muquit/software/mailsend/mailsend.html
Priority: optional
Architecture: i386
Depends: #{@depends}
Description: Send mail via SMTP protocol
 mailsend is a simple command line program to send mail via SMTP protocol. 
 Among other things it supports ESMTP authentication and MIME attachments.
 STARTTLS and SMTP over SSL is also supported.  It was originally written 
 for Microsoft Windows but later on ported to Linux/Unix. The Homepage of
 mailsend is at: http://muquit.com/muquit/software/mailsend/mailsend.html
 License is GNU GPL.


EOD
    f.close
    rescue => ex
      log "Could not create '#{@control_file}' (#{ex})"
    end
  end

  def gen_copyright
    f = File.open("./debian/usr/share/doc/mailsend/copyright", "w")
    year = Time.new.year
    f.puts <<EOD
mailsend

Copyright #{year} Muhammad Muquit <muquit@muquit.com>

License: GNU GPL

On Debian systems, the full text of the GPL can be found at
/usr/share/common-licenses/GPL

EOD
    f.close
  end

  def compress(file)
    cmd = "gzip --force --best #{file}"
    system(cmd)
  end

  def strip_binary(file)
    cmd = "strip #{file}"
    system(cmd)
  end
  

  def copy_files
    FileUtils.copy("./mailsend", "./debian/usr/bin")
    strip_binary("./debian/usr/bin/mailsend")
    FileUtils.copy("./doc/mailsend.1.gz", "./debian/usr/share/man/man1")

    file = "./debian/usr/share/doc/mailsend/changelog"
    FileUtils.copy("./changelog.debian", file)
    compress(file)

    file = "./debian/usr/share/doc/mailsend/changelog.Debian"
    FileUtils.copy("./changelog.debian", file)
    compress(file)
  end

  def make_dirs
    @dirs.each do |dir|
      log "Make directory: #{dir}"
      FileUtils.mkdir_p(dir)
    end
  end

  def lintian
    package_name = get_package_name
    log "Running lintian on #{package_name}"
    cmd = "lintian #{package_name}"
    system(cmd)
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

  def get_package_name
    pname = "mailsend_#{@mailsend_ver}-ubuntu_i386.deb"
    @package_name = pname
    return pname
  end

  def make_package
    package_name = get_package_name
    FileUtils.remove("./debian.deb") if File.exists?("./debian.deb")
    cmd = "fakeroot dpkg-deb --build debian"
    log "Running: #{cmd}"
    system(cmd)
    FileUtils.mv("debian.deb", package_name)
    log "Package: #{package_name}"
  end

  def print_install_inst
  puts <<EOD
##########################################
  
To install the package, type:
  sudo dpkg -i #{@package_name}

To remove the package, type:
  sudo dpkg -r mailsend

##########################################

EOD
  end

  def doit
    check
    make_dirs
    gen_control_file
    gen_copyright
    copy_files
    make_package
    lintian
    print_install_inst
  end

end

if __FILE__ == $0
  deb = MakeDebianPackage.new
  deb.doit
end

