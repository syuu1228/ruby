#encoding:utf-8
puts "th0 : id -> -1"
th0 = Thread.new do
    th0.access_control_id = -1
    require "open-uri"
    require "cgi"
end
th0.join

puts "th1 : id -> 0"
th1 = Thread.new do
    th1.access_control_id = 0
    end_flag = true
    begin
      require "open-uri"
    rescue
      puts :ErrorX
      end_flag = false
    end
    exit if end_flag
    require "cgi"
end
th1.join

puts "th2 : id -> 1"
th2 = Thread.new do
    th2.access_control_id = 1
    end_flag = true
    begin
      require "open-uri"
    rescue
      puts :Error
      end_flag = false
    end
    require "cgi"
end
th2.join
