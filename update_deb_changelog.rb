#!/usr/bin/env ruby

########################################################
# update the debian changelog file with dch. 
#  To udpdate the changelog file:
#    ./update_deb_changelog.rb 
#  To increment the version:
#    ./update_deb_changelog.rb blah
# muquit@muquit.com May-11-2012 
########################################################
class UpdateDebianChangelog

  def initialize
    ENV['CHANGELOG'] = './changelog.debian'
    ENV['DEBFULLNAME'] = 'Muhammad Muquit'
    ENV['DEBEMAIL'] =  'muquit@muquit.com'
    ENV['VISUAL'] = 'vi'
  end

  def doit
    if ARGV.length > 0
      system("dch -i")
    else
      system("dch")
    end
  end
end

if __FILE__ == $0
  dcl = UpdateDebianChangelog.new.doit
end

