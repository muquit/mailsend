#Introduction
**mailsend** is a simple command line program to send mail via [SMTP](http://www.ietf.org/rfc/rfc0821.txt?number=821) protocol. I needed to send a piece of alert mail from a program in a networked Windows machine, but could not find a simple program like this installed. So I wrote one. You might find it useful in some situations. The program does not use any config file and I plan to keep it that way.  

**From version 1.17b1+, IPv6 is supported in all platforms.**

Suggestions, bug reports are always welcome. 

If you have questions, concerns, suggestions, please join the discussion group [http://groups.google.com/group/mailsend-discuss mailsend-discuss] instead of sending me mails.

If you are a competent coder, you are welcome to join the project and make it better.

#Supported Platforms#

  * Microsoft Windows
  * Linux/Unix
  * Mac OS X

It should compile on any POSIX compliant system.  Works on 64 bit systems.
#Versions#
##Latest stable version is 1.16##
Released on Mar-4-2012.  ChangeLog
##Latest Development version is 1.17b14
Released on Oct-20-2013 ChangeLog. Just because it a development version, it does not mean it is unstable.  This version has many bug fixes which is not available in v1.16.

#Synopsis

    Version: @(#) mailsend v1.16

    Copyright: GNU GPL. It is illegal to use this software for Spamming

    (Compiled with OpenSSL 1.0.0g 18 Jan 2012)
    usage: mailsend [options]
    Where the options are:
     -smtp hostname/IP*    - Hostname/IP address of the SMTP server
     -port SMTP port       - SMTP port
     -d    domain          - domain name for SMTP HELO/EHLO
     -t    to,to..*        - email address/es of the recipient/s
     -cc   cc,cc..         - Carbon copy address/es
     +cc                   - do not ask for Carbon Copy
     -bc   bcc,bcc..       - Blind carbon copy address/es
     +bc                   - do not ask for Blind carbon copy
     +D                    - don not add Date header
     -f    address*        - email address of the sender
     -sub  subject         - subject
     -l    file            - a file containing the email addresses
     -attach file,mime_type,[i/a] (i=inline,a=attachment)
                           - attach this file as attachment or inline
     -cs   character set   - for text/plain attachments (default is us-ascii)
     -M    "one line msg"  - attach this one line text message
     -name "Full Name"     - add name in the From header
     -v                    - verbose mode
     -V                    - show version info
     -w                    - wait for a CR after sending the mail
     -rt  email_address    - add Reply-To header
     -rrr email_address    - request read receipts to this address
     -ssl                  - SMTP over SSL
     -starttls             - Check for STARTTLS and if server supports, do it
     -auth                 - Try CRAM-MD5,LOGIN,PLAIN in that order
     -auth-cram-md5        - use AUTH CRAM-MD5 authentication
     -auth-plain           - use AUTH PLAIN authentication
     -auth-login           - use AUTH LOGIN authentication
     -user username        - username for ESMTP authentication
     -pass password        - password for ESMTP authentication
     -example              - show examples
     -ehlo                 - force EHLO
     -info                 - show SMTP server information
     -help                 - shows this help
     -q                    - quiet

The options with `*` must the specified
You must know the FQDN or IP address of the SMTP server you use. The file you specify with -l contains email address one per line. For determining MIME types, please look at [mimetypes MIME Types].

     Version: @(#) mailsend v1.17b14

     Copyright: BSD. It is illegal to use this software for Spamming

     (Compiled with OpenSSL version: OpenSSL 1.0.0g 18 Jan 2012)
     usage: mailsend [options]
     Where the options are:
      -copyright            - show copyright information
      -smtp hostname/IP*    - Hostname/IP address of the SMTP server
      -port SMTP port       - SMTP port
      -d    domain          - domain name for SMTP HELO/EHLO
      -t    to,to..*        - email address/es of the recipient/s
      -cc   cc,cc..         - Carbon copy address/es
      +cc                   - do not ask for Carbon Copy
      -ct   seconds         - Connect timeout. Default is 5 seconds
      -bc   bcc,bcc..       - Blind carbon copy address/es
      +bc                   - do not ask for Blind carbon copy
      +D                    - don not add Date header
      -f    address*        - email address of the sender
      -sub  subject         - subject
      -lilst file           - a file containing a list of email addresses
      -log file             - write log messages to this file
      -attach file,mime_type,[i/a] (i=inline,a=attachment)
                            - attach this file as attachment or inline
      -cs   character set   - for text/plain attachments (default is us-ascii)
      -enc  type            - Encoding Type. Only valid type: base64
      -H    "header"        - Add custom Header
      -M    "one line msg"  - attach this one line text message
      -name "Full Name"     - add name in the From header
      -v                    - verbose mode
      -show_attach          - show attachment in verbose mode, default is no
      -V                    - show version info
      -w                    - wait for a CR after sending the mail
      -rt  email_address    - add Reply-To header
      -rrr email_address    - request read receipts to this address
      -ssl                  - SMTP over SSL
      -starttls             - Check for STARTTLS and if server supports, do it
      -auth                 - Try CRAM-MD5,LOGIN,PLAIN in that order
      -auth-cram-md5        - use AUTH CRAM-MD5 authentication
      -auth-plain           - use AUTH PLAIN authentication
      -auth-login           - use AUTH LOGIN authentication
      -user username        - username for ESMTP authentication
      -pass password        - password for ESMTP authentication
      -example              - show examples
      -ehlo                 - force EHLO
      -info                 - show SMTP server information
      -help                 - shows this help
      -q                    - quiet
The options with `*` must be specified. Environment variables: **SMTP_USER_PASS** for plain text password (-pass)

# Examples
Please visit [examples Examples] at the  wiki section
# How to compile/install
## Linux/Unix/Mac OS X
    $ /bin/sh ./configure --with-openssl=/usr

In Mac OS X Mavericks as of Oct-27-2013, you have to do the following:

    $ /bin/sh ./configure --with-openssl=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr

Default installation location is /usr/local. If you want to change that:

    $ /bin/ch ./configure --with-openssl=/usr --prefix=/usr/local/mailsend
    $ make clean
    $ make
    # make install
    Or
    $ sudo make install
    Or
    $ make install DESTDIR=/foo


The name of the binary will be mailsend. If compiled with OpenSSL, the support for STARTTLS and SMTP over SSL will be available.
## Linux Debian package
A Debian package is also supplied. To install the package, type: 

    $ sudo dpkg -i mailsend_1.17b14-ubuntu_i386.deb

To uninstall, type:

    $ sudo dpkg -r mailsend 

If you prefer to build the Debian package yourself, after compiling, type: 

    $ ruby ./mk_debian_package.rb 

## Microsoft Windows
A compiled binary for MS Windows is already supplied. However, if you need to modify something or want to compile it yourself, you will need MS Visual Studio (I used MS Visual Studio 10, freely available from Microsoft).  You have to find and run the vsvars32.bat file that came with Visual Studio first in order to use VS from command line. To compile, open a command shell and type:

    c:\> nmake -f Makefile.nmake clean
    c:\> nmake -f Makefile.nmake

It compiles cleanly with cygwin as well. The name of the binary will be mailsend.exe 

# Known Issues
  * For text or html attachments, \r\n is not added at the end of lines. Some SMTP servers (e.g. qmail) does not like that. It will be fixed in some future release.
  * For non ascii characters, MIME headers are not converted as per RFC2047
  * In Windows, non ascii characters can not be used with -M. In Linux/Mac for non-ascii characters for -M, also specify -enc base64 (available in v1.17b13+)

# FAQ
Please visit [mailsendFAQ FAQ] at the wiki section
