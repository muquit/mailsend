[![Downloads](https://img.shields.io/github/downloads/muquit/mailsend/total.svg)](https://github.com/muquit/mailsend/releases)
## Table Of Contents
- [Announcement (Feb-11-2019)](#announcement-feb-11-2019)
- [mailsend v1.19](#mailsend-v119)
  - [Download source/binary](#download-sourcebinary)
  - [Supported Platforms](#supported-platforms)
  - [Features](#features)
  - [Versions](#versions)
    - [Latest stable version is 1.19](#latest-stable-version-is-119)
  - [Synopsis](#synopsis)
  - [How to compile/install](#how-to-compileinstall)
    - [Linux/Unix/Mac OS X](#linuxunixmac-os-x)
    - [Linux Debian package](#linux-debian-package)
    - [Microsoft Windows](#microsoft-windows)
  - [Known Issues](#known-issues)
  - [FAQ](#faq)
  - [Examples](#examples)
  - [License](#license)

# Announcement (Feb-11-2019)

If you use mailsend, please try [mailsend-go](https://github.com/muquit/mailsend-go). Your feedback will be appreciated.

mailsend will still be maintained (critical security fixes) but no new features will be added.

Thanks!

# mailsend v1.19

**mailsend** is a simple command line program to send mail via [SMTP](http://www.ietf.org/rfc/rfc0821.txt?number=821) protocol. Long time ago, I needed to send a piece of alert mail from a program in a networked Windows machine, but could not find a simple program like this installed. So I wrote one. You might find it useful as well. The program does not use any config file and I plan to keep it that way. **From version 1.17b1+, IPv6 is supported in all platforms.** Suggestions, bug reports are always welcome. If you have any question, request or suggestion, please enter it in the [Issues](https://github.com/muquit/mailsend/issues/) with appropriate label.

## Download source/binary

To download source/binaries, please go to the [releases](https://github.com/muquit/mailsend/releases/) page.

Bug fixes and stable features are merged from dev branch to master branch every now and then. Clone the repo if you want the latest code.

## Supported Platforms

* Microsoft Windows
* Linux/Unix
* Mac OS X

It should compile on any POSIX compliant system. Works on 64 bit systems.

## Features
Few main features:

* ESMTP Authentication
* IPv6 transport
* Attachments
* Embed images in HTML
* Send mail to a list of users

## Versions
### Latest stable version is 1.19

Released on May-17-2016. Please look at: [ChangeLog](ChangeLog.mediawiki).

## Synopsis

```
 Version: @(#) mailsend v1.19

 Copyright: BSD. It is illegal to use this software for Spamming

 (Compiled with OpenSSL version: OpenSSL 1.0.2f  28 Jan 2016)
 usage: mailsend [options]
 Where the options are:
  -copyright            - show copyright information
  -4                    - Force to use IPv4 address of SMTP server
  -6                    - Force to use IPv6 address of SMTP server
  -smtp hostname/IP*    - Hostname/IP address of the SMTP server
  -port SMTP port       - SMTP port
  -domain    domain     - domain name for SMTP HELO/EHLO
  -t    to,to..*        - email address/es of the recipient/s
  -cc   cc,cc..         - carbon copy address/es
  +cc                   - do not ask for Carbon Copy
  -ct   seconds         - Connect timeout. Default is 5 seconds
  -read-timeout seconds - Read timeout. Default is 5 seconds
  -bc   bcc,bcc..       - blind carbon copy address/es
  +bc                   - do not ask for Blind carbon copy
  +D                    - do not add Date header
  -f    address*        - email address of the sender
  -sub  subject         - subject
  -list_address file    - a file containing a list of email addresses
  -log file             - write log messages to this file
  -cs   character set   - for text/plain attachments (default is us-ascii)
  -separator character  - separator used with -attach. Default is comma (,)
                          If used must be specified before -attach
  -enc-type type        - encoding type. base64, 8bit, 7bit etc.
                          Default is base64. Special type is "none"
  -aname name           - name of the attachment. Default is filename
  -content-id id        - content-id in the attachment
  -mime-type type       - MIME type
  -dispostion val       - "attachment" or "inline". Default is "attachment"
  -attach file,mime_type,[i/a] (i=inline,a=attachment)
                        - attach this file as attachment or inline
  -show-attach          - show attachment in verbose mode, default is no
  -show-mime-types      - show the compiled in MIME types
  -M    "one line msg"  - attach this one line text message
  -content-type type    - Content type. Default: multipart/mixed
  -msg-body path        - Path of the file to include as body of mail
  -embed-image image    - Path of image to embed in HTML
  -H    "header"        - Add custom Header
  -name "Full Name"     - add name in the From header
  -v                    - verbose mode
  -V                    - show version info
  -w                    - wait for a CR after sending the mail
  -rt  email_address    - add Reply-To header
  -rrr email_address    - request read receipts to this address
  -rp                   - return-path address
  -ssl                  - SMTP over SSL
  -starttls             - use STARTTLS if the server supports it
  -auth                 - try CRAM-MD5,LOGIN,PLAIN in that order
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

 The options with * must be specified
 Environment variables:
  SMTP_USER_PASS for plain text password (-pass)
```

The options with asterisk must be specified. Environment variable **SMTP_USER_PASS** can be used to set the  SMTP authentication password instead of the option +-pass+

## How to compile/install

### Linux/Unix/Mac OS X

Specify the path of OpenSSL with +--with-openssl+. In most Linux systems, it is installed in +/usr+. If OpenSSL is installed in non-standard places, type +./configure --help+ for options.

```
   $ /bin/sh ./configure --with-openssl=/usr
```

Default installation location is +/usr/local+. If you want to change that:

```
   $ /bin/sh ./configure --with-openssl=/usr --prefix=/usr/local/mailsend
   $ make clean
   $ make
   # make install
   Or
   $ sudo make install
   Or
   $ make install DESTDIR=/foo
```

The name of the binary will be mailsend. If compiled with OpenSSL, the
support for STARTTLS and SMTP over SSL will be available.

### Linux Debian package

Starting from mailsend v1.18, Debian package is no longer supplied. If you want to build the Debian package yourself, after compiling mailsend, type:

```
   $ ruby scripts/mk_debian_package.rb
```

To install the package, type:

```java
   $ sudo dpkg -i mailsend_1.19-ubuntu_i386.deb
```

To uninstall, type:

```java
   $ sudo dpkg -r mailsend
```

### Microsoft Windows

A compiled binary with SMTP authentication support for MS Windows is already supplied. However, if you need to modify something or want to compile it yourself, you will need freely available Visual Studio from Microsoft. For Visual Studio 2015 community edition, start **Developer Command Prompt for VS2015**.

OpenSSL is required, if you need to compile with SMTP authentication support. Look at +INSTALL.32+ that comes with OpenSSL on how to compile and install OpenSSL. mailsend’s +Makefile.nmake+ expects it to be installed at +c:\openssl+

To compile mailsend, open a command shell and type:

```
   c:\> nmake -f Makefile.nmake clean
   c:\> nmake -f Makefile.nmake
```

It compiles cleanly with cygwin as well. The name of the binary will be +mailsend.exe+

## Known Issues

* For text or html attachments, \r\n is not added at the end of lines. Some SMTP servers (e.g. qmail) does not like that. It will be fixed in some future release.
* For non ascii characters, MIME headers are not converted as per RFC2047
* In Windows, non ascii characters can not be entered in subject or with +-M+ from
command line (Works fine in Linux and Mac). **mailsend** uses standard C
string functions and Windows versions do not know about multibyte input
from command line ([Issue# 14](https://github.com/muquit/mailsend/issues/14)). If anyone is familiar with it, you are welcome to implement the support, unfortunately I do not have time at this time.

## FAQ

Please look at [mailsend FAQ](doc/mailsend-FAQ.mediawiki)

## Examples

Please type: +mailsend -example+ or look at [examples](doc/examples.mediawiki)

## License

BSD

To display license, please type: `mailsend -copyright`

```
Copyright (C) 2001-2025  Muhammad Muquit (http://www.muquit.com/)

 o Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

 o Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

 o Neither the name of the author MUHAMMAD MUQUIT (http://www.muquit.com/)
nor the names of its contributors may be used to endorse or promote
products derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
```

---
<sub>TOC is created by https://github.com/muquit/markdown-toc-go on Apr-05-2025</sub>
