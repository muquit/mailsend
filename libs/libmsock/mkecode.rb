#!/usr/bin/env ruby
# muquit@muquit.com Aug-26-2012 
begin
    t = Time.new
    puts <<EOD
\#ifndef WERRNO_H
\#define WERRNO_H

\#ifdef WINNT

/*
** This file is auto generated on #{t}
** Map wisock error code. Newer Visual Studio defines them
** but they do not map to Winsock error code.
*/
EOD

  lines = File.read("./ecode.txt")

  lines.each do |l|
    l = l.strip
    if l =~ /#define (.*)\s+(.*)$/
      d = $1.strip
      puts <<EOD
\#undef #{d}
#{l}

EOD
    end
  end
  puts <<EOD
\#endif /* WINNT */

\#endif /* WERRNO_H */
EOD
end
