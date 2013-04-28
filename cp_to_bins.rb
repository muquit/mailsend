#!/usr/bin/env ruby

# copy to mailsned_binary and add to svn
# muquit@muquit.com Mar-16-2013 
require 'fileutils'

begin
  version = nil
  l = `./mailsend -V 2>&1`
  lines = l.split("\n")
  lines.each do |line|
    line = line.chomp
    line = line.strip
    if line =~ /.*mailsend v(.*)$/
      version = $1
    end
  end

  if version
    to_dir = "../mailsend_bins/#{version}"
    if !File.exists?(to_dir)
      puts "Creating dir: #{to_dir}"
      FileUtils.mkdir_p(to_dir)
    end
    files = []
    files << "mailsend-#{version}.tar.gz"
    files << "mailsend#{version}.exe.zip"
    files << "mailsend_#{version}-ubuntu_i386.deb"

    files.each do |file|
      if !File.exists?(file)
        puts "Error: File #{file} does not exist"
        exit 1
      end
      puts "File: #{file} -> #{to_dir}"
      FileUtils.cp(file,to_dir)
    end
  else
    puts "Could not determine version"
  end
rescue => e
  puts "Error: #{e}"
end
