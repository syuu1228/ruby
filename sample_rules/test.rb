add_method_rule do
  list_mode = :blacklist

  list  = ["BlackClass.blackFunction"]
end

add_method_rule do
  list_mode = :whitelist
  
  list = ["BlackClass2.blackFunction2",
   "BlackClass3.blackFunction3"]
end

add_require_rule do
  list_mode = :blacklist

  list = ["twitter"]
end

add_require_rule do
  list_mode = :whitelist

  list = ["oauth",
          "json"]
end
