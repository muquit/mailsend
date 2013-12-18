#!/usr/bin/env ruby

# print the date in debian changelog format
# Example: Wed, 28 Mar 2012 20:45:31 -0400
# muquit@muquit.com Mar-28-2012 

require 'date'

offset = DateTime.now.zone.to_s
offset = offset.sub(/:/,'')
t = Time.new
d = t.strftime("%a, %d %b %Y %H:%M:%S #{offset}")
puts " -- Muhammad Muquit <muquit@muquit.com>  #{d}"
