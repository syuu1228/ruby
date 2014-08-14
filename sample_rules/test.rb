add_method_rule do
  list_mode = :blacklist

  list  = ["BlackClass.blackFunction",
           "BlackClass2.blackFunction2"]
end

add_require_rule do
  list_mode = :blacklist

  list = ["twitter",
          "oauth"]
end
