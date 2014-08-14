#encoding:utf-8
class BlackClass
    def blackFunction
      puts "BlackList::blackFunction"
    end
end
class BlackClass2
    def blackFunction2
      puts "BlackList2::blackFunction2"
    end
end
puts "th0 : id -> -1"
th0 = Thread.new do
    th0.access_control_id = -1
    t = BlackClass.new
    t.blackFunction
    t = BlackClass2.new
    t.blackFunction2
end
th0.join

puts "th1 : id -> 0"
th1 = Thread.new do
    th1.access_control_id = 0

    t = BlackClass.new
    end_flag = true
    begin
      t.blackFunction
    rescue
      puts :Error
      end_flag = false
    end
    exit if end_flag
    t = BlackClass2.new
    t.blackFunction2
end
th1.join

puts "th2 : id -> 1"
th2 = Thread.new do
    th2.access_control_id = 1
    
    end_flag = true
    begin
      t = BlackClass.new
      t.blackFunction
    rescue
      puts :Error
      end_flag = false
    end
    exit if end_flag
    t = BlackClass2.new
    t.blackFunction2
end
th2.join

