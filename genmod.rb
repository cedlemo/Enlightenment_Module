#!/usr/bin/env ruby
require 'fileutils'
require 'getoptlong'

opts = GetoptLong.new(
  [ '--help', '-h', GetoptLong::NO_ARGUMENT ],
  [ '--author', '-a', GetoptLong::OPTIONAL_ARGUMENT ],
  [ '--mail', '-m', GetoptLong::OPTIONAL_ARGUMENT]
)

def usage()
  puts <<-EOF
usage : boostrap.rb [OPTIONS] module_name
  -h, --help:   show help
  -a, --author: set the author name, $USER by default
  -m, --mail:   set the author mail $USER@localhost by default
EOF
end

author=ENV['USER']
mail=author + '@localhost'

opts.each do |opt, arg|
  case opt
    when '--help'
      usage
    when '--author'
      author = arg
    when '--mail'
      mail = arg
  end
end
if ARGV.count != 1
  usage
  exit 0
end

MODULE_NAME = ARGV[0]
AUTHOR = author
MAIL = mail
MODULE_DIR= File.expand_path("./") + "/" + ARGV[0]
PATTERN="skel"
PATTERN_ALT="Skel"
PATTERN_AUTHOR="skel_author_name" 
PATTERN_MAIL="skel_author_mail"

def parse_file(item, new_dir)
  new_file_name = new_dir + "/" + File.basename(item.gsub(/#{PATTERN}/, MODULE_NAME))
  if !(`file --mime-type -b #{item}`.gsub(/\n/,"") =~ /^text.*/)
    FileUtils.cp(item, new_file_name)
  else
    new_file = File.new(new_file_name, "w")
    File.open(item) do |file|
      file.each_line do |line|
        n_line = line.gsub(/#{PATTERN}/, MODULE_NAME)
        n_line = n_line.gsub(/#{PATTERN_ALT}/, MODULE_NAME.capitalize)
        n_line = n_line.gsub(/#{PATTERN_AUTHOR}/, AUTHOR)
        n_line = n_line.gsub(/#{PATTERN_MAIL}/, MAIL)
        new_file.puts(n_line)
      end
    end
    new_file.close
  end
  File.chmod( 0744, new_file_name) if File.executable?(item)
end

def parse_files(dir, main_dir = false)
  dir = File.expand_path(dir)
  new_dir = nil
  if main_dir
    new_dir = MODULE_DIR
  else
    new_dir = MODULE_DIR + "/" + File.basename(dir)
  end
  FileUtils.mkdir(new_dir)

  Dir.glob(dir + "/*").each do |item|
    if File.directory?(item)
      parse_files(item)
    elsif File.file?(item)
      parse_file(item, new_dir)
    end
  end
end

parse_files('./skel', true )
