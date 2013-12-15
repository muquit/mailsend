#!/usr/bin/env ruby

##################################################
# make examples.c file from misc/examples.
# muquit@muquit.com Dec-14-2013 
##################################################
#
require 'fileutils'

ME = __FILE__
while File.symlink?(ME)
    ME= File.expand_path(File.readlink(ME), File.dirname(ME))
end
MYDIR = File.expand_path(File.dirname(ME))

class MakeExamples
  EXAMPLE_FILE = MYDIR + "/misc/examples.txt"

  def initialize
  end

  def log(msg)
    t = Time.new
    puts "#{t}: #{msg}"
  end

  def make_examples
    v = `./mailsend -V 2>&1`
    va = v.split("\n")
    puts <<-EOF
#include "mailsend.h"
void show_examples(void)
{
  (void) fprintf(stdout, "Examples of #{va[0]}\\n\\n");
  (void) fprintf(stdout,
EOF
    lines = File.read(EXAMPLE_FILE)
    lines.each do |line|
    line = line.chomp
    line = line.gsub(/\"/,'\"')
      if line.length == 0
        puts " \"\\n\""
      else
        puts " \" #{line}\\n\""
      end
    end
    puts <<-EOF
);
}
EOF
 end

  def doit
    make_examples
  end

end

if __FILE__ == $0
  MakeExamples.new.doit
end

