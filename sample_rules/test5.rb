add_method_rule do
  list_mode = :blacklist

  list  = ["BlackClass.blackFunction"]
end

add_method_rule do
  list_mode = :whitelist
  
  list = ["BlackClass2.blackFunction2",
          "Class.new",
          "Kernel.puts",
          "Kernel.require"]
end

add_require_rule do
  list_mode = :blacklist

  list = ["open-uri"]
end

add_require_rule do
  list_mode = :whitelist

  list = ["cgi"]
          
end
