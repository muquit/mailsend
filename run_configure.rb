#!/usr/bin/env ruby

# If config.log exists, try to determine how the configure was run last time
# and generate Makefiles. 
# muquit@muquit.com Apr-18-2012 
class RunConfigure

  def doit
    cmd = "sh ./configure"
    file = "./config.log"
    if File.exists?(file)
      f = File.open(file, "r")
      while (line = f.gets)
        line = line.chomp
        #   $ ./configure --with-openssl=/usr
        if line =~ /^\s+\$ (.*configure.*)$/
          cmd = $1
          puts "line: #{line}\n";
          break
        end
      end
      f.close
    end
    puts cmd
    system(cmd)
  end

end

if __FILE__ == $0
  r = RunConfigure.new
  r.doit
end
