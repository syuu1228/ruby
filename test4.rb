th0 = Thread.new do
    th0.access_control_id = -1
    t = BlackList.new
    t.blackFunction
end
th0.join

th1 = Thread.new do
    th1.access_control_id = 1
    t = BlackList.new
    t.blackFunction
end
th1.join
