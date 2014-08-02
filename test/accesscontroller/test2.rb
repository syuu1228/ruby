#encoding:utf-8
class BlackList
  def blackFunction
    puts "BlackList::blackFunction"
  end
end

bl = BlackList.new
bl.blackFunction
