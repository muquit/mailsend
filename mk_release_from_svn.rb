#!/usr/bin/env ruby

########################################################################
# A script to create distribution from SVN.
#
# Create a tar.gz file of a software from a svn trunk. The tree must have
# trunk and tags directories. The current version from trunk will
# be tagged with the supplied version and tar file will be
# project_name<version>.tar.gz. 
#
# Usage:  mk_release_svn.rb <version>
# Example:
#   mk_release_svn.rb 1.18
#
# muquit@muquit.com Mar-17-2013 
########################################################################

require 'fileutils'

class MakeRelease
  ME = $0
  VERSION = "1.02"

  def initialize
    @svn_url = ''
    @version = ''
    @project_name = ''
    @tar_file = ''
    @dir = ''
  end

  ##--------------------------------------------------- 
  # Do svn info and find the base url
  # If the end of the url is trunk, turnk word is removed
  # to determine the base. Populates the @svn_url 
  ##--------------------------------------------------- 
  def find_svn_url
    x = `svn info`
    lines = x.split("\n")
    lines.each do |line|
      line = line.chomp
      if line =~ /^URL: (.*)$/
        url = $1
        puts url
        if url =~ /(.*)\/trunk$/
          @svn_url = $1
        else
          @svn_url = url
        end
        return @svn_url
      end
    end
  end

  def log(msg)
    t = Time.new
    Kernel.print "#{msg}"
  end

  def check_args
    if ARGV.length != 1
      puts <<EOD
  Usage: #{ME} <verion to release>
  Example:
  #{ME} 1.16
  
  The latest version from trunk will be tagged as 1.16 inside the tags 
  directory and a tar file project-1.16.tar.gz will be created by exporting
  the tagged version 1.16. The base directory of the tar file will be 
  project-1.16

  A branch 1.16 also will also be created in the branches directory.

EOD
    exit 1
    end
    find_svn_url
    log "SVN URL: #{@svn_url}\n"
    @version = ARGV[0]

    @project_name = File.basename(@svn_url)
    @dir = @project_name + "-" + @version
    @tar_file = @dir + ".tar.gz"

    log "Project: #{@project_name}\n"
    log "Dir: #{@dir}\n"
    log "tgz file: #{@tar_file}\n"
  end

  def check_exitstatus(cmd, rc)
    if rc != 0
      log "Comamnd '#{cmd}' failed\n"
      exit 1
    end
  end

  def run_command(cmd)
    cmd = cmd + " >/dev/null 2>&1"
    system(cmd)
    return $?.exitstatus
  end

  def check_trunk_and_tags
    url = @svn_url + "/trunk"
    log "Checking trunk...#{url}: "
    cmd = "svn info #{url}"
    rc = run_command(cmd)
    check_exitstatus(cmd, rc)
    log "OK\n"

    url = @svn_url + "/tags"
    log "  Checking tags...#{url}: "
    cmd = "svn info #{url}"
    rc = run_command(cmd)
    check_exitstatus(cmd, rc)
    log "OK\n"
  end

  def verify_svn
    check_trunk_and_tags
  end

  def tag_current_version
    trunk = @svn_url + "/trunk"
    tag = @svn_url + "/tags" + "/" + @version
    cmd = "svn info #{tag}"
    rc = run_command(cmd)
    if rc != 0
      log "Tagging current version to #{@version}... "
      cmd = "svn copy -m \"tagging v#{@version}\" #{trunk} #{tag}"
      rc = run_command(cmd)
      check_exitstatus(cmd, rc)
      log "OK\n"
    else
      log "Tag #{tag} already exists\n"
    end
  end

  def branch_current_version
    trunk = @svn_url + "/trunk"
    branches = @svn_url + "/branches" + "/" + @version
    cmd = "svn info #{branches}"
    rc = run_command(cmd)
    if rc != 0
      log "Brancing current version to #{@version}... "
      cmd = "svn copy -m \"branching v#{@version}\" #{trunk} #{branches}"
      rc = run_command(cmd)
      check_exitstatus(cmd, rc)
      log "OK\n"
    else
      log "Branch #{branches} already exists\n"
    end

  end

  def export_tag
    url = @svn_url + "/tags" + "/" + @version
    if !File.exists?(@dir)
      log "Exporing...\n"
      log "export tag: #{url}"
      cmd = "svn export #{url} #{@dir}"
      log "#{cmd}"
      rc = run_command(cmd)
      check_exitstatus(cmd, rc)
      log "OK\n"
    else
      log "Directory '#{@dir}' already exists\n"
    end
  end

  def make_tar_gz
    if File.exists?(@dir)
      tar_file = @dir + ".tar.gz"
      log "Making #{tar_file} ..."
      cmd = "tar -czf #{tar_file} #{@dir}"
      rc = run_command(cmd)
      check_exitstatus(cmd, rc)
      log "OK\n"
      system("/bin/ls -lt #{tar_file}")
    end
  end

  def remove_dir
    if File.exists?(@dir)
      log "Removing directory #{@dir}"
      FileUtils.rm_rf(@dir)
    end
  end

  def doit
    check_args
    verify_svn
    branch_current_version
    tag_current_version
    export_tag
    make_tar_gz
    remove_dir
  end
end

if __FILE__ == $0
  m = MakeRelease.new
  m.doit
end
