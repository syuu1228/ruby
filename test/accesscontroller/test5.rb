#encoding:utf-8
puts "th0 : id -> -1"
th0 = Thread.new do
    th0.access_control_id = -1
    require "json"
end
th0.join

puts "th1 : id -> 1"
th1 = Thread.new do
    th1.access_control_id = 1
    require "json"
    end
th1.join
