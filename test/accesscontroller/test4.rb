#encoding:utf-8
class BlackClass
    def blackFunction
      puts "BlackList::blackFunction"
    end
end
puts "th0 : id -> -1"
th0 = Thread.new do
    th0.access_control_id = -1
    t = BlackClass.new
    t.blackFunction
end
th0.join

puts "th1 : id -> 1"
th1 = Thread.new do
    th1.access_control_id = 1
    t = BlackClass.new
    t.blackFunction
end
th1.join
